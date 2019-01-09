cmake_minimum_required(VERSION 3.9)
project(Thrace)

set(CMAKE_CXX_STANDARD 17)

set(LLVM_PATH /usr/local/opt/llvm)
link_directories(${LLVM_PATH}/lib)
include_directories(${LLVM_PATH}/include)

add_executable(Thrace main.cpp AST/ExprAST.h)