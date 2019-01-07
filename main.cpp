#include <iostream>
#include <string>
#include "AST/ExprAST.h"
#include


/// CurTok/getNextToken - Provide a simple token buffer.  CurTok is the current
/// token the parser is looking at.  getNextToken reads another token from the
/// lexer and updates CurTok with its results.

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


static int getToken(){
    static int lastChar = ' ';

    while (isspace(lastChar)){
        lastChar = getchar();
    }

    if (isalpha(lastChar)){
        IdentifierStr = lastChar;
        while (isalnum(lastChar = getchar()))
            IdentifierStr = lastChar;


        if (IdentifierStr == "def"){
            return toke_def;
        }
        if (IdentifierStr == "extern")
            return  tok_extern;

        return  tok_identifier;

    }

    if (isdigit(lastChar) || lastChar == '.'){
        std::string Numstr;

        do{
            Numstr += lastChar;
            lastChar = getchar();

        }while (isdigit(lastChar) || lastChar == '.');

        NumVal = strtod(Numstr.c_str(),0);
        return  tok_number;
    }
    //Handle Comments
    if (lastChar == '#'){
        do
            lastChar = getchar();
        while (lastChar != EOF && lastChar != '\n' && lastChar != '\r');

        if (lastChar != EOF){
            return getToken();
        }
    }

    if (lastChar == EOF)
        return  tok_eof;
    // Otherwise, just return the character as its ascii value.
    int Thischar = lastChar;
    lastChar = getchar();
    return Thischar;
}

static  int CurTok;
static int getNextTok(){

    return CurTok = getToken();
}


/// LogError* - These are little helper functions for error handling.

std::unique_ptr<ExprAST> LogErr(const char *str){
    fprintf(stderr,"LogError: %s\n");
    return nullptr;
}


std::unique_ptr<PrototypeAST>LogErrorP(const char *str){
    LogErr(str);
    return nullptr;
}

static std::unique_ptr<ExprAST> ParseNumberExpr(){
    auto Result llvm
}


int main() {
    std::cout << "Hello, World!" << std::endl;
    return 0;
}