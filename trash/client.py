# -*- coding: UTF-8 -*-
# 文件名：client.py

import time
import socket               # 导入 socket 模块

s = socket.socket()         # 创建 socket 对象
host = socket.gethostname() # 获取本地主机名
port = 8888                # 设置端口好

s.connect(("10.1.1.111", port))

#s.send(b"chenzhongyao")


while 1:
    data = s.recv(1024)
    for i in data:
        print(ord(i))

    s.send(b"ok!")

    data = s.recv(1024)

    for i in data:
        print(ord(i))

    s.send(b"ok!")
    time.sleep(1)

while 1:
    pass

data = s.recv(1024)


s.close()  
