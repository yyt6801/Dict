//#include "stdafx.h"
//注意找个代码，会报错 大概是因为注释导致文件终止，删除掉大段的注释。。。
#include <WinSock2.h>
#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string>

#pragma comment(lib,"ws2_32")

#define HALO_S_PORT     9090
#define MIN_BUF 128

/*
LPVOID是一个没有类型的指针，也就是说你可以将LPVOID类型的变量赋值给任意类型的指针，
比如在参数传递时就可以把任意类型传递给一个LPVOID类型为参数的方法，
然后在方法内再将这个“任意类型”从传递时的“LPVOID类型”转换回来。
*/

//把收到的请求发送给alarm_server(127.0.0.1:9091)
int SEND_ALARM_SERVER(char * sendData)
{
	WORD sockVersion = MAKEWORD(2,2);
	WSADATA data; 
	if(WSAStartup(sockVersion, &data) != 0)
	{
		return 0;
	}

	SOCKET sclient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(sclient == INVALID_SOCKET)
	{
		printf("invalid socket !");
		return 0;
	}

	sockaddr_in serAddr;
	serAddr.sin_family = AF_INET;
	serAddr.sin_port = htons(9091);
	serAddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1"); 
	if (connect(sclient, (sockaddr *)&serAddr, sizeof(serAddr)) == SOCKET_ERROR)
	{
		printf("connect error !");
		closesocket(sclient);
		return 0;
	}
	send(sclient, sendData, strlen(sendData), 0);

	char recData[255];
	int ret = recv(sclient, recData, 255, 0);
	if(ret > 0)
	{
		recData[ret] = 0x00;
		printf(recData);
	}
	closesocket(sclient);
	WSACleanup();
}


//这个线程recv请求，处理请求，再将内容发到fd上去
static DWORD CALLBACK FdHandler(IN LPVOID lpCtx) {		//这是一个线程函数 //将fd传入到这里

	SOCKET fd = (SOCKET)lpCtx;
	char  *Buffer = new char[8193];
	int    i = recv(fd, Buffer, 8192, 0);			//从TCP的另一端接收数据，返回的是实际copy的字节数
	if (i > 0) {

		Buffer[i] = '\0';
		char method[MIN_BUF];
		char url[MIN_BUF];
		char *h = new char[i + 1];
		strcpy(h, Buffer);
		printf("\r\n%s\r\n", Buffer);
		SEND_ALARM_SERVER(h);

		//
		// http request parsing, and generate http response
		//

		int i, j;
		//example:GET /su?wd=ww&action=opensearch&ie=UTF-8 HTTP/1.1\r\n
		//处理接收数据
		i = 0; j = 0;
		// 取出第一个单词，一般为HEAD、GET、POST
		while (!(' ' == Buffer[j]) && (Buffer[j]!= '\0'))
		{
			method[i] = Buffer[j];
			i++; j++;
		}
		method[i] = '\0';   // 结束符，这里也是初学者很容易忽视的地方

		printf("method: %s\n", method);
		// 如果不是GET或HEAD方法，则直接断开本次连接
		// 如果想做的规范些可以返回浏览器一个501未实现的报头和页面
		if (stricmp(method, "GET") && stricmp(method, "HEAD"))//比较，相等返回0
		{
			printf("not get or head method.\n");
			printf("***********************\n\n\n\n");
			closesocket(fd);
			return 0;
		}
		i = 0;
		while ((' ' == Buffer[j]))
			j++;
		while (!(' ' == Buffer[j]))
		{
			url[i] = Buffer[j];
			i++; j++;
		}
		url[i] = '\0';
		printf("url: %s\n",url);
		char if_api_word[20] = "/api/word=";
		char rec_api_word[20];
		char rec_search_word[20];
		int if_url_correct=0;
		for (;if_url_correct<10;if_url_correct++)
		{
			strncpy(rec_api_word,url,strlen(if_api_word));
			
		}
		rec_api_word[10] = '\0';

		if (strcmp(rec_api_word,if_api_word) != 0)
		{
			printf("cannot parsing request.url=%s\n",url);
			printf("***********************\n\n\n\n");
			closesocket(fd);
			return 0;
		}
		else{
			for(;if_url_correct<strlen(url);if_url_correct++)
				strncpy(rec_search_word,url+strlen(if_api_word),strlen(url)-strlen(if_api_word));
			rec_search_word[strlen(url)-strlen(if_api_word)] = '\0';
			printf("要查询的英文单词为：rec_search_word=%s\n",rec_search_word);

			//开始调用dll查询翻译
			char send_all[4096];
			memset(send_all,0x00,sizeof(send_all));
			char *ex_words;
			HINSTANCE h = LoadLibrary("test_dll");
			char *(*psearch_words)(char *);
			psearch_words = (char *(_cdecl *)(char *))(GetProcAddress(h,"search_words"));
			
			ex_words=psearch_words(rec_search_word);

			//printf("ex_words = %s\n",ex_words);//返回的是ex_words
			//printf("strlen(ex_words) = %d\n",strlen(ex_words));//返回的是ex_words

			//拼凑出返回的response
			char header[180]=
				"HTTP/1.1 200 OK\r\n"
				"Content-Type: text/html; charset=gb2312\r\n"
				"Access-Control-Allow-Origin: *\r\n"
				"Connection: Close\r\n\r\n";

			strcpy(send_all,header);
			strcat(send_all,ex_words);
			printf("strlen(send_header) = %d\n",strlen(send_all));//返回的是ex_words
			send_all[strlen(send_all)+strlen(ex_words)] = '\0';
			//printf("send:%s\n",send_all);
			printf("size:%d\r\n%s\r\n", strlen(send_all)+strlen(ex_words), send_all);

			i = send(fd, send_all, 4096, 0);
			FreeLibrary(h);
		}

		/*i = sprintf(Buffer,
			"HTTP/1.1 200 OK\r\n"
			"Content-Type: text/html\r\n"
			"Connection: Close\r\n\r\n"
		);*/

		

// 		FILE *pf = fopen("halos_html.txt", "r");
// 		i += fread(&Buffer[i], 1, 8192 - i, pf);		//pf内容读入buffer
// 		/*函数原型
// size_t fread ( void *buffer, size_t size, size_t count, FILE *stream) ;
// buffer 用于接收数据的内存地址
// size   要读的每个数据项的字节数，单位是字节
// count  要读count个数据项，每个数据项size个字节.
// stream 输入流
// 返回值 返回真实读取的项数，若大于count则意味着产生了错误。*/

// 		Buffer[i] = '\0';
// 		fclose(pf);

// 		char *p = strstr(Buffer, "%_HALO_%");//strstr(str1, str2) 函数用于判断字符串str2是否是str1的子串。 如果是，则该函数返回str2在str1中首次出现的地址；否则，返回NULL。
// 		if (p) {								//??????????????????????????

// 			*p = '\0';
// 			std::string tmp = Buffer;
// 			tmp += h;
// 			tmp += &p[0];
// 			i = (int)tmp.size();
// 			memcpy(Buffer, tmp.c_str(), i);//由src指向地址为起始地址的连续n个字节的数据复制到以destin指向地址为起始地址的空间内。

// 			Buffer[i] = '\0';

// 		}
		//printf("size:%d\r\n%s\r\n", i+strlen(send_data), Buffer);			//打印halos_html.txt的内容

		//i = send(fd, Buffer, i+strlen(send_data), 0);
	}
	delete[]Buffer;
	Sleep(1000);
	closesocket(fd);
	return 0;
}

int main(int argc, char* argv[])
{
	WSADATA wsaData;
	WSAStartup(0x0202, &wsaData);
	SOCKADDR_IN addr;
	SOCKET lfd = socket(PF_INET, SOCK_STREAM, 0);
	int i;

	addr.sin_addr.S_un.S_addr = 0;
	addr.sin_family = PF_INET;
	addr.sin_port = htons(HALO_S_PORT);

	i = bind(lfd, (const sockaddr*)&addr, sizeof(SOCKADDR_IN));
	if (i != NOERROR) {
		printf("Oops, failed to bind:%d port\r\n", HALO_S_PORT);
		return 0;
	}
	listen(lfd, 1970);		//三次握手过程中等待包的最大数量，防SYN_FLOOD攻击
	do {
		i = sizeof(SOCKADDR_IN);
		SOCKET fd = accept(lfd, (sockaddr*)&addr, &i);
		if (fd != INVALID_SOCKET) {
			printf("\r\ncoming from [%s:%d]\r\n", inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));//这行要验证一下作用 addr不是本机的吗
					//端口输出的时候要转换为host格式

			char requert_info[2048];
			sprintf(requert_info,"coming from [%s:%d]", inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));
			SEND_ALARM_SERVER(requert_info);
			HANDLE hThread = CreateThread(NULL, 0,
				FdHandler,
				(LPVOID)fd,
				0,
				NULL);
			CloseHandle(hThread);
/*
HANDLE CreateThread(
                    LPSECURITY_ATTRIBUTES lpThreadAttributes,指向SECURITY_ATTRIBUTES的指针，用于定义新线程的安全属性，一般设置成NULL；
                    DWORD dwStackSize,分配以字节数表示的线程堆栈的大小，默认值是0；
                    LPTHREAD_START_ROUTINE lpStartAddress,指向一个线程函数地址。每个线程都有自己的线程函数，线程函数是线程具体的执行代码；
                    LPVOID lpParameter,传递给线程函数的参数；
                    DWORD dwCreationFlags,表示创建线程的运行状态，其中CREATE_SUSPEND表示挂起当前创建的线程，而0表示立即执行当前创建的进程；
                    LPDWORD lpThreadID返回新创建的线程的ID编号；
                   );
				   如果函数调用成功，则返回新线程的句柄，
*/
		}
		else {
			printf("Something is wrong\r\n");		//出现wrong 的情况 没等到第三个响应包，对方掉网了或者SYN_FLOOD攻击
		}
	} while ("Go ahead");

	WSACleanup();
	return 0;
}