# -*- coding: utf-8 -*-
"""
Spyder Editor
by Kumar Aman

"""

import socket

server = input("Provide the domain address to resolve: ")

print("IP: ", socket.gethostbyname(server))