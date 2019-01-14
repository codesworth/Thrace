//
// Created by Mensah Shadrach on 14/01/2019.
//

#ifndef THRACE_BASICDIAGNOSTICS_H
#define THRACE_BASICDIAGNOSTICS_H
#include "../AST/ExprAST.h"

class BasicDiagnostics {
public:
    BasicDiagnostics();
    std::unique_ptr<ExprAST> LogErr(const char *str);
    std::unique_ptr<PrototypeAST>LogErrorP(const char *str);
};


#endif //THRACE_BASICDIAGNOSTICS_H
