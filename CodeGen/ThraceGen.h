//
// Created by Mensah Shadrach on 28/01/2019.
//

#ifndef THRACE_THRACEGEN_H
#define THRACE_THRACEGEN_H

#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/IRBuilder.h>


class ThraceGen {

public:
    ThraceGen() = default;
    llvm::LLVMContext TheContext;
    llvm::IRBuilder<> Builder(TheContext);
    s


};


#endif //THRACE_THRACEGEN_H
