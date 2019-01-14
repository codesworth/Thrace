//
// Created by Mensah Shadrach on 14/01/2019.
//

#include <llvm/ADT/STLExtras.h>

#include <utility>
#include <map>
#include "BasicParser.h"



BasicParser::BasicParser():Diagnostics(BasicDiagnostics()),Lexer(BaseLexer()){
    BinopPrecedence['<'] = 10;
    BinopPrecedence['+'] = 20;
    BinopPrecedence['-'] = 20;
    BinopPrecedence['*'] = 40;
}

std::unique_ptr<ExprAST> BasicParser::ParseNumberExpr(){
    auto Result = llvm::make_unique<NumberExprAST>(NumVal);
    Lexer.getToken();
    return std::move(Result);
}


/// parenexpr ::= '(' expression ')'
std::unique_ptr<ExprAST> BasicParser::ParseParenExpr() {
    CurTok = Lexer.getNextTok(); // eat (.
    auto V;  //ParseExpression();
    if (!V)
        return nullptr;

    if (CurTok != ')')
        return Diagnostics.LogErr("expected ')'");
    CurTok = Lexer.getNextTok(); // eat ).
    return V;
}


/// identifierexpr
///   ::= identifier
///   ::= identifier '(' expression* ')'

std::unique_ptr<ExprAST> BasicParser::ParseIdentifierExpr(){
    std::string IdName = std::move(IdentifierStr);
    CurTok = Lexer.getNextTok();

    if (CurTok != '('){
        //Simple Variable Ref
        return  llvm::make_unique<VariableExprAST>(IdName);

    }

    //Call
    CurTok = Lexer.getNextTok();
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
               Diagnostics.LogErr("Expected ')' or ',' in argument list");

            CurTok = Lexer.getNextTok();
        }


    }
    //Eat the ')'
    CurTok = Lexer.getNextTok();

    llvm::make_unique<CallExprAST>(IdName,std::move(Args));

}


/// primary
///   ::= identifierexpr
///   ::= numberexpr
///   ::= parenexpr

std::unique_ptr<ExprAST>BasicParser::ParsePrimary(){
    switch (CurTok){
        default:
            return Diagnostics.LogErr("unknown token when expecting an expression");
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



/// GetTokPrecedence - Get the precedence of the pending binary operator token.

int BasicParser::getTokPrecedence(){
    if (isascii(CurTok))
        return -1;

    // Make sure it's a declared binop.
    int tok_precedence = BinopPrecedence[CurTok];
    if (tok_precedence <= 0)
        return -1;

    return tok_precedence;
}

/// expression
///   ::= primary binoprhs
///
/// \return

std::unique_ptr<ExprAST>BasicParser::ParseExpression(){
    auto LHS = ParsePrimary();
    if (!LHS)
        return nullptr;

    //return ParseBinOpRHS(0,std::move(LHS));
}
