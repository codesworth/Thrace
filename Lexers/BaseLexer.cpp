
// Created by Mensah Shadrach on 14/01/2019.
//


#include "BaseLexer.h"


int BaseLexer::getToken(){
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

int BaseLexer::getNextTok(){

    return getToken();
}

BaseLexer::BaseLexer()  = default;
