// #include "stdafx.h"
#include "stdio.h"
#include <Windows.h>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <io.h>
extern "C"
{
	class _declspec(dllexport) A
	{
	public:
		A()
		{
			printf("A construct...\n");
		}
		const char *whoIsMe()
		{
			return "Myname is A";
		}
		~A()
		{
			printf("A deconstruct...\n");
		}
	};
	_declspec(dllexport) A a;
#define MAX_LINE 3800
#define IMGNUM 20000 //图片所在文件夹中图片的最大数量
	char img_files[IMGNUM][1000];

	int getFiles(char *path)
	{
		int num_of_img = 0;
		long hFile = 0;
		struct _finddata_t fileinfo;
		char p[700] = {0};
		strcpy(p, path);
		strcat(p, "\\*");
		if ((hFile = _findfirst(p, &fileinfo)) != -1)
		{
			do
			{
				if ((fileinfo.attrib & _A_SUBDIR))
				{
					if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
						continue;
				}
				else
				{
					//strcpy(img_files[num_of_img], path);
					//strcpy(img_files[num_of_img], "\\");
					//strcat(img_files[num_of_img], "\\");
					//strcat(img_files[num_of_img++], fileinfo.name);
					strcpy(img_files[num_of_img++], fileinfo.name);
				}
			} while (_findnext(hFile, &fileinfo) == 0);
			_findclose(hFile);
		}
		return num_of_img;
	}
	_declspec(dllexport) char *search_words(char *aim_code)
	{
		//	char aim_code[20];
		//scanf("%s", aim_code);

		char root[180];
		memset(root, 0x00, sizeof(root));

		char xxx[1];
		memset(xxx, 0x00, sizeof(xxx));

		strcpy(root, "DICT/");

		if (aim_code[0] >= 97 && aim_code[0] <= 122)
			xxx[0] = aim_code[0] - 32;

		root[strlen(root)] = xxx[0];
		strcat(root, "/");

		//
		int num = getFiles(root);
		int i, j;
		if (1 == num)
		{
			strcat(root, img_files[0]);
		}
		else
		{
			for (i = 0; i < num; ++i)
			{
				//printf("%s\n", img_files[i]);
				for (j = 2; j < 20; j++)
				{
					if (img_files[i][j] == aim_code[1])
					{
						strcat(root, img_files[i]);
						printf("%s\n", root);
						break;
						break;
					}
				}
			}
		}

		/*root[41] = aim_code[0];
	strcat(root, "-");
	root[43] = aim_code[1];
	strcat(root, ".txt");*/
		//printf("%s\n", root);
		//system("pause");

		char buf[MAX_LINE]; /*缓冲区*/
		FILE *fp;			/*文件指针*/
		int len;			/*行字符个数*/
		int rownum = 0;
		if ((fp = fopen(root, "r")) == NULL)
		{
			printf("cannot read %s", root);
			return "cannot translate this words!";
			//system("pause");
			/*perror("fail to read");
        exit(1);*/
		}
		while (fgets(buf, MAX_LINE, fp) != NULL)
		{
			len = strlen(buf);
			buf[len - 1] = '\0'; /*去掉换行符*/

			rownum++;
			if (strcmp(buf, aim_code) == 0)
			{
				fgets(buf, MAX_LINE, fp);
				printf("%s %d \n", buf, rownum);
				//system("pause");

				return buf;
			}
		}
		return "no this words!";
		printf("cannot get password!\n");
	}
	_declspec(dllexport) int g_nUsageCount = 3195;
}