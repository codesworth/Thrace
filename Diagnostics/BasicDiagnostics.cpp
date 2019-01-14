//
// Created by Mensah Shadrach on 14/01/2019.
//

#include "BasicDiagnostics.h"




/// LogError* - These are little helper functions for error handling.

std::unique_ptr<ExprAST> BasicDiagnostics::LogErr(const char *str){
    fprintf(stderr,"LogError: %s\n",str);
    return nullptr;
}

std::unique_ptr<PrototypeAST> BasicDiagnostics::LogErrorP(const char *str) {
    LogErr(str);
    return nullptr;
}

BasicDiagnostics::BasicDiagnostics() = default;


