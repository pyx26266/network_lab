 
cmake_minimum_required(VERSION 2.8)

project("tic tac toe" C)

add_executable(server.out game_server.c)

add_executable(client.out game_client.c)
