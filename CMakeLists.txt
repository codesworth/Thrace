cmake_minimum_required(VERSION 3.9)
project(Thrace)

set(CMAKE_CXX_STANDARD 17)

set(LLVM_PATH /usr/local/opt/llvm)
link_directories(${LLVM_PATH}/lib)
include_directories(${LLVM_PATH}/include)

add_executable(Thrace main.cpp AST/ExprAST.h Parsing/BasicParser.cpp Parsing/BasicParser.h Diagnostics/BasicDiagnostics.cpp Diagnostics/BasicDiagnostics.h Lexers/BaseLexer.cpp Lexers/BaseLexer.h)