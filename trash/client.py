# -*- coding: UTF-8 -*-
# 文件名：client.py

import socket               # 导入 socket 模块

s = socket.socket()         # 创建 socket 对象
host = socket.gethostname() # 获取本地主机名
port = 8888                # 设置端口好

s.connect(("10.1.1.111", port))

#s.send(b"chenzhongyao")

data = s.recv(1024)

for i in data:
    print(ord(i))

data = s.recv(1024)


s.close()  
