#include <iostream>
#include <string>


enum Token{
    tok_eof = -1,

    //Commands
    toke_def = -2,
    tok_extern = -3,

    //primary
    tok_identifier = -4,
    tok_number = -5
};

static  std::string IdentifierStr;
static  double  NumVal;



int main() {
    std::cout << "Hello, World!" << std::endl;
    return 0;
}