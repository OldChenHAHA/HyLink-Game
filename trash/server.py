# -*- coding: UTF-8 -*-
# 文件名：server.py
from __future__ import print_function
import socket  # 导入 socket 模块


def get_host_ip():
    try:
        s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        s.connect(('8.8.8.8', 80))
        ip = s.getsockname()[0]
    finally:
        s.close()
    return ip


s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)  # 创建 socket 对象
hostname = socket.gethostname()  # 获取本地主机名
port = 8888  # 设置端口
s.bind((hostname, port))  # 绑定端口

print((get_host_ip(), port))

s.listen(5)  # 等待客户端连接
while True:
    c, addr = s.accept()  # 建立客户端连接。
    print('Connected address：%s', end='')
    print(addr)
    print( c.recv(1024) )
    
    c.send(b'Welcome to Windows !')

    c.close()  # 关闭连接

