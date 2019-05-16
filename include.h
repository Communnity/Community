//
// Created by Jiyang Gong on 2019-03-26.
//

#ifndef Communtity_INCLUDE_H
#define Communtity_INCLUDE_H


#include<vector>
#include<iostream>
#include<algorithm>
#include<cstdlib>
#include<cstring>
#include<string>
#include<unordered_map>
#if defined __GNUC__ || defined __APPLE__
#include <ext/hash_map>
#else
#include <hash_map>
#endif

#ifdef __APPLE__
    #include <sys/uio.h>
#else
    #include <sys/io.h>
#endif
#include <dirent.h>
#include<fstream>

using namespace std;
using namespace __gnu_cxx;
void read(int &x);
void read(double &x);
char* StrToChar(string str);
#endif //Communtity_INCLUDE_H
