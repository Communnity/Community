//
// Created by Jiyang Gong on 2019-03-26.
//

#include "include.h"

void read(int &x)
{
    int f=1;x=0;char s=getchar();
    while(s<'0'||s>'9'){if(s=='-')f=-1;s=getchar();}
    while(s>='0'&&s<='9'){x=x*10+s-'0';s=getchar();}
    x*=f;

}

char* StrToChar(string str){
    char file_s[100] = "";
    strcpy(file_s, str.c_str());
    return file_s;
}

int ran(int md){
    return rand() % md + 1;
}