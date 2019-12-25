# 创建TCP服务器示例：
# 1、创建socket对象
# 2、绑定主机和端口,应当是一个元组
# 3、监听
# 4、等待连接
# 5、接受数据
# 6、发送数据
# 7、关闭套接字

import socket
import time

# 明确配置变量
ip_port = ('127.0.0.1', 9091)
back_log = 5
buffer_size = 1024
# 创建一个TCP套接字
ser = socket.socket(
    socket.AF_INET,
    socket.SOCK_STREAM)  # 套接字类型AF_INET, socket.SOCK_STREAM tcp协议，基于流式的协议
ser.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)  # 对socket的配置重用ip和端口号
# 绑定端口号
ser.bind(ip_port)  # 写哪个ip就要运行在哪台机器上
# 设置半连接池
ser.listen(back_log)  # 最多可以连接多少个客户端
while 1:
    # 阻塞等待，创建连接
    con, address = ser.accept()  # 在这个位置进行等待，监听端口号
    while 1:
        try:
            # 接受套接字的大小，怎么发就怎么收
            msg = con.recv(buffer_size)
            # if msg.decode('utf-8') == '1':
            # 断开连接
            con.close()
            # 写文件
            # localtime = time.asctime(time.localtime(time.time()))
            with open("test.txt", "a") as out_file:
                out_file.write('\n\n' +
                               time.asctime(time.localtime(time.time())) +'\n'+
                               msg.decode('utf-8'))
            print('服务器收到消息', msg.decode('utf-8'))
        except Exception as e:
            break
# 关闭服务器
ser.close()
