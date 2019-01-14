//
// Created by Mensah Shadrach on 14/01/2019.
//

#ifndef THRACE_BASELEXER_H
#define THRACE_BASELEXER_H
#include <string>

enum Token{
    tok_eof = -1,

    //Commands
            toke_def = -2,
    tok_extern = -3,

    //primary
            tok_identifier = -4,
    tok_number = -5
};


class BaseLexer {

public:
    BaseLexer();
    int getToken();
    double NumVal;
    std::string IdentifierStr;

    int getNextTok();
};


#endif //THRACE_BASELEXER_H
