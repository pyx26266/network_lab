#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Spyder Editor
by Kumar Aman

"""
import sys
import socket

if (len(sys.argv) < 2):
    server = input("Provide the domain address to resolve: ")
else:
    server = sys.argv[1]
    
print("IP: ", socket.gethostbyname(server))