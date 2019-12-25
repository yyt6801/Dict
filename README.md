# Dict
《牛津英汉词典》 查词
借用文本词典实现查词翻译功能，学习练手的好项目
功能：   查词
-------------------
##### 1.后台C/C++ 实现在词典文本查词功能，涉及文件夹遍历，字符串拼接、处理、查找等；
##### 2.C/C++实现API接口，可接受HTTP请求，并对请求解析，涉及SOCKET通讯；
##### 3.调用dll动态链接库，实现数据传入和数据输出；
##### 4.新增用python实现http服务端，监听发送来的请求和内容并保存为本地日志；

------------------------
### 文件说明：
##### DICT文件夹：   《牛津英汉词典》txt文本字典；
##### http_Server1.exe：   HTTPServer，监听9090端口，并对请求参数进行解析；调用dll；返回结果；发送日志给日志服务Server(python);
##### TCP_socket_server.py:   新增python实现的HTTPServer,用作日志服务器，记录http_Server1的请求日志；
##### test_dll.dll：   实现查词功能的动态链接库，接收要查的单词，返回该单词的完整翻译；

-------------------------
### HOW TO USE
##### 1.编译http_Server1,建议vs环境
##### 2.编译test_dll为test_dll.dll,建议vs环境，若需在其他环境运行需设置 `Use MFC in a Static Library`
##### 3.运行http_Server1.exe,开始测试...
