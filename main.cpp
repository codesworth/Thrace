#include <iostream>
#include "Parsing/BasicParser.h"


int main() {
    BasicParser Parser = BasicParser();

    Parser.MainLoop();
    std::cout << "Hello, World!" << std::endl;
    return 0;
}