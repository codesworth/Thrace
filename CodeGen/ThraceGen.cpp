//
// Created by Mensah Shadrach on 28/01/2019.
//

#include "ThraceGen.h"


ThraceGen::ThraceGen():Builder(TheContext) {

}



Value * ThraceGen::LogErrorV(const char* s){
    fprintf(stderr,"LogError: %s\n",s);
    return nullptr;
}