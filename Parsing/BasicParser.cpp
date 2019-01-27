//
// Created by Mensah Shadrach on 14/01/2019.
//
#include "llvm/ADT/STLExtras.h"

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
    getNextToken(); // eat (.
    auto V = ParseExpression();
    if (!V)
        return nullptr;

    if (CurTok != ')')
        return Diagnostics.LogErr("expected ')'");
    getNextToken(); // eat ).
    return V;
}


/// identifierexpr
///   ::= identifier
///   ::= identifier '(' expression* ')'

std::unique_ptr<ExprAST> BasicParser::ParseIdentifierExpr(){
    std::string IdName = std::move(IdentifierStr);
    getNextToken();

    if (CurTok != '('){
        //Simple Variable Ref
        return  llvm::make_unique<VariableExprAST>(IdName);

    }

    //Call
    getNextToken();
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

           getNextToken();
        }


    }
    //Eat the ')'
    getNextToken();

    return  llvm::make_unique<CallExprAST>(IdName,std::move(Args));

}


/// primary
///   ::= identifierexpr
///   ::= numberexpr
///   ::= parenexpr

std::unique_ptr<ExprAST>BasicParser::ParsePrimary(){
    switch (CurTok){
        case tok_identifier:
            return ParseIdentifierExpr();
        case tok_number:
            return ParseNumberExpr();
        case '(':
            return ParseParenExpr();
        default:
            return Diagnostics.LogErr("unknown token when expecting an expression");
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

    return ParseBinOpRHS(0,std::move(LHS));
}

/// binoprhs
///   ::= ('+' primary)*

std::unique_ptr<ExprAST> BasicParser::ParseBinOpRHS(int ExprPrec, std::unique_ptr<ExprAST> LHS) {

    // If this is a binop, find its precedence.
    while (1){
        int TokPrecedence = getTokPrecedence();

        // If this is a binop that binds at least as tightly as the current binop,
        // consume it, otherwise we are done.

        if (TokPrecedence < ExprPrec){
            return LHS;
        }

        // Okay, we know this is a binop.
        int BinOp = CurTok;
        getNextToken(); //
        auto RHS = ParsePrimary();
        if (!RHS)
            return nullptr;

        int NextPrec = getTokPrecedence();
        if (TokPrecedence < NextPrec){
            RHS = ParseBinOpRHS(TokPrecedence + 1,std::move(RHS));
            if (!RHS)
                return nullptr;
        }

        // Merge LHS/RHS.
        LHS = llvm::make_unique<BinaryExprAST>(BinOp,std::move(LHS), std::move(RHS));

    }// loop around to the top of the while loop.

    // If BinOp binds less tightly with RHS than the operator after RHS, let
// the pending operator take RHS as its LHS.

}


int BasicParser::getNextToken(){
   CurTok = Lexer.getNextTok();
    return CurTok;
}



/// prototype
///   ::= id '(' id* ')'

std::unique_ptr<PrototypeAST>BasicParser::ParsePrototype(){
    if (CurTok!= tok_identifier)
        Diagnostics.LogErr("Expected Function Name");
    std::string fnName = IdentifierStr;
    getNextToken();

    if (CurTok != '('){
        return Diagnostics.LogErrorP("Expected '(' after function Name" );
    }

    // Read the list of argument names.
    std::vector<std::string> ArgsNames;
    while (getNextToken() == tok_identifier){
        ArgsNames.push_back(IdentifierStr);

    }

    if (CurTok != ')')
        return Diagnostics.LogErrorP("Expected ')' in prototype");

    getNextToken();

    return llvm::make_unique<PrototypeAST>(fnName,std::move(ArgsNames));
}



/// definition ::= 'def' prototype expression

std::unique_ptr<FunctionAST> BasicParser::ParseDefinition(){
     getNextToken(); //eat def

    auto proto = ParsePrototype();

    if (!proto)
        return nullptr;

    if (auto E = ParseExpression())
        return llvm::make_unique<FunctionAST>(std::move(proto), std::move(E));

    return nullptr;
}


/// external ::= 'extern' prototype

std::unique_ptr<PrototypeAST> BasicParser::ParseExtern(){
    getNextToken();
    return ParsePrototype();

}

/// toplevelexpr ::= expression

std::unique_ptr<FunctionAST> BasicParser::ParseTopLevelExpr(){

    if (auto E = ParseExpression()){
        // Make an anonymous proto.
        auto Proto = llvm::make_unique<PrototypeAST>("",std::vector<std::string>());
        return llvm::make_unique<FunctionAST>(std::move(Proto), std::move(E));
    }

    return nullptr;
    
}


//Handlers

void BasicParser::HandleDefinition(){
    if (ParseDefinition()){
        fprintf(stderr,"Parsed a function definition");
    } else{
        //Skip TOken for error recovery
        getNextToken();
    }
}

void BasicParser::HandleExtern(){

    if (ParseExtern()) {
        fprintf(stderr, "Parsed an extern\n");
    } else {
        // Skip token for error recovery.
       getNextToken();
    }
}

void BasicParser::HandleTopLevelExpression(){

    // Evaluate a top-level expression into an anonymous function.
    if (ParseTopLevelExpr()) {
        fprintf(stderr, "Parsed a top-level expr\n");
    } else {
        // Skip token for error recovery.
        getNextToken();
    }
}


//The Driver for thelanguage
/// top ::= definition | external | expression | ';'


void BasicParser::MainLoop(){

    while (1){
        std::fprintf(stderr,"Ready>");
        switch (CurTok){
            case tok_eof:
                return;
            case ';':
                getNextToken();
                break;
            case toke_def:
                HandleDefinition();
                break;
            case tok_extern:
                HandleExtern();
                break;
            default:
                HandleTopLevelExpression();
                break;

        }
    }
}


