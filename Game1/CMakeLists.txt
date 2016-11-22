cmake_minimum_required(VERSION 3.4)
project(Game1)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread ")

set(SOURCE_FILES main.cpp CGameBoard.cpp CTorus.cpp )
add_executable(Game1 ${SOURCE_FILES})