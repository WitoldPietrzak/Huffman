cmake_minimum_required(VERSION 3.15)
project(Tele3)

set(CMAKE_CXX_STANDARD 14)
link_libraries(ws2_32 wsock32)
add_executable(Tele3 main.cpp binaryTree.cpp binaryTree.h codeMessage.cpp codeMessage.h binaryInputOutput.cpp binaryInputOutput.h)