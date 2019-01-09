#include <iostream>
#include <string>
#include "AST/ExprAST.h"
#include <llvm/Analysis/AliasAnalysis.h>




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
    auto Result = llvm::make_unique<NumberExprAST>(NumVal);
    getToken();
    return std::move(Result);
}


std::unique_ptr<ExprAST> ParseExpression(){
    return 0;
}


/// parenexpr ::= '(' expression ')'
static std::unique_ptr<ExprAST> ParseParenExpr() {
    getNextTok(); // eat (.
    auto V;  //ParseExpression();
    if (!V)
        return nullptr;

    if (CurTok != ')')
        return LogErr("expected ')'");
    getNextTok(); // eat ).
    return V;
}


/// identifierexpr
///   ::= identifier
///   ::= identifier '(' expression* ')'

static std::unique_ptr<ExprAST> ParseIdentifierExpr(){
    std::string IdName = IdentifierStr;
    getNextTok();

    if (CurTok != '('){
        //Simple Variable Ref
        return  llvm::make_unique<VariableExprAST>(IdName);

    }

    //Call
    getNextTok();
    std::vector<std::unique_ptr<ExprAST>> Args;

    if (CurTok != ')'){
        while (1){
            if (auto Arg = ParseExpression())
               Args.push_back(std::move(Arg));
            else
                return nullptr;

            if (CurTok == ')')
                break;

            if (CurTok != ',')
                LogErr("Expected ')' or ',' in argument list");

            getNextTok();
        }


    }
    //Eat the ')'
    getNextTok();

    llvm::make_unique<CallExprAST>(IdName,std::move(Args));

}


/// primary
///   ::= identifierexpr
///   ::= numberexpr
///   ::= parenexpr

static std::unique_ptr<ExprAST> ParsePrimary(){
    switch (CurTok){
        default:
            return LogErr("unknown token when expecting an expression");
        case tok_identifier:
            return ParseIdentifierExpr();
        case tok_number:
            return ParseNumberExpr();
        case '(':
            ParseParenExpr();
    }
}

/// BinopPrecedence - This holds the precedence for each binary operator that is
/// defined.

static std::map<char,int> BinopPrecedence;

/// GetTokPrecedence - Get the precedence of the pending binary operator token.

static int getTokPrecedence(){
    if (isascii(CurTok))
        return -1;

    // Make sure it's a declared binop.
    int tok_precedence = BinopPrecedence[CurTok];
    if (tok_precedence <= 0)
        return -1;

    return tok_precedence;
}



int main() {
    BinopPrecedence['<'] = 10;
    BinopPrecedence['+'] = 20;
    BinopPrecedence['-'] = 20;
    BinopPrecedence['*'] = 40;
    std::cout << "Hello, World!" << std::endl;
    return 0;
}