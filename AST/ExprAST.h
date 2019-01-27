//
// Created by Mensah Shadrach on 07/01/2019.
//

#include <string>
#include <vector>


class ExprAST{

public:
    virtual ~ExprAST(){}

};


class NumberExprAST:public ExprAST{
    double  Val;

public:
    NumberExprAST(double val):Val(val){}
};


/// VariableExprAST - Expression class for referencing a variable, like "a".

class VariableExprAST: public ExprAST{
    std::string Name;

public:
    VariableExprAST(const std::string &Name): Name(Name){}
};

/// BinaryExprAST - Expression class for a binary operator.

class BinaryExprAST:public ExprAST{

    char Op;
    std::unique_ptr<ExprAST> LHS,RHS;

public:
    BinaryExprAST(char op, std::unique_ptr<ExprAST>LHS, std::unique_ptr<ExprAST> RHS):
            Op(op),LHS (std::move(LHS)), RHS(std::move(RHS)){}


};


/// CallExprAST - Expression class for function calls.

class CallExprAST:public ExprAST{

    std::string Callee;
    std::vector<std::unique_ptr<ExprAST>> Args;


public:
    CallExprAST(std::string &callee, std::vector<std::unique_ptr<ExprAST>> Args):
            Callee(callee),Args(std::move(Args)){}


};


/// PrototypeAST - This class represents the "prototype" for a function,
/// which captures its name, and its argument names (thus implicitly the number
/// of arguments the function takes).


class PrototypeAST{

    std::string Name;
    std::vector<std::string>Args;


public:
    PrototypeAST(std::string name, std::vector<std::string> args):
            Name(name), Args(args){}

    std::string &getName (){
        return Name;
    }

};



/// FunctionAST - This class represents a function definition itself.

class FunctionAST{

    std::unique_ptr<PrototypeAST> Proto;
    std::unique_ptr<ExprAST> Body;

public:
    FunctionAST(std::unique_ptr<PrototypeAST> proto, std::unique_ptr<ExprAST> body):
      Proto(std::move(proto)), Body(std::move(body)){}
};

