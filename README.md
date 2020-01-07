# Dict
《牛津英汉词典》 查词，输入英文单词，可翻译对应的音标,翻译,词组和例句
借用文本词典实现查词翻译功能，学习练手的好项目
## 功能：  翻译、HttpServer、日志保存

http_Server1.exe 可以接收前端或其他http get请求：
#### 请求示例：http://127.0.0.1:9090/api/word=like   
#### 返回示例： / lUk; lJk/ v 1 [I, Ipr, Ip] ~ (at sb/sth) turn one's eyes in a particular direction (in order to see sb/sth) 看; 瞧; 望: If you look carefully you can just see the church from here. 你仔细看的话, 可以从这里看到那座教堂. * We looked but saw nothing. 我们看了...


-------------------
### 功能说明：
##### 1.后台C/C++ 实现在词典文本查词功能，涉及文件夹遍历，字符串拼接、处理、查找等；
##### 2.C/C++实现API接口，可接受HTTP请求，并对请求解析，涉及SOCKET通讯；
##### 3.调用dll动态链接库，实现数据传入和数据输出(test_dll.dll为功能实现的核心部分，为方便调用编译为动态链接库)  
    当查不到对应的词时，返回 cannot translate this words!
##### 4.新增TCP_socket_server.py： 用python实现的http服务端，监听9091端口，接收http_Server1发送来的请求和内容并保存为本地日志；  
    把日志保存到本地，保存到本地的test.txt文档，保存请求时间，请求ip，请求url，请求的word，查询结果

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


--------------------------
### TO DO
##### 1.做一个前端，输入需要查询的单词，可显示出翻译的结果。输入框+查询按钮；ajax发送请求+返回结果显示