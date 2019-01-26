//
// Created by Mensah Shadrach on 14/01/2019.
//

#ifndef THRACE_BASICPARSER_H
#define THRACE_BASICPARSER_H
#include "../AST/ExprAST.h"
#include "../Diagnostics/BasicDiagnostics.h"
#include "../Lexers/BaseLexer.h"

class BasicParser {

public:
    BasicParser();
    void MainLoop();

private:
    std::unique_ptr<ExprAST> ParseNumberExpr();
    std::unique_ptr<ExprAST> ParseParenExpr();
    std::unique_ptr<ExprAST> ParseIdentifierExpr();
    int getTokPrecedence();
    std::unique_ptr<ExprAST> ParseExpression();
    std::unique_ptr<ExprAST> ParsePrimary();
    int CurTok;
    BasicDiagnostics Diagnostics;
    BaseLexer Lexer;
    double NumVal;
    std::string IdentifierStr;
    std::map<char,int> BinopPrecedence;
    std::unique_ptr<ExprAST> ParseBinOpRHS(int ExprPrec, std::unique_ptr<ExprAST> RHS);

    std::unique_ptr<PrototypeAST> ParsePrototype();

    std::unique_ptr<FunctionAST> ParseDefinition();

    std::unique_ptr<PrototypeAST> ParseExtern();

    std::unique_ptr<FunctionAST> ParseTopLevelExpr();

    void HandleDefinition();

    void HandleExtern();

    void HandleTopLevelExpression();
};


#endif //THRACE_BASICPARSER_H
