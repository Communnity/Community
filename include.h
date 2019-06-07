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
#include<map>
#if defined __GNUC__ || defined __APPLE__
#include <ext/hash_map>
#else
#include <hash_map>
#endif

#ifdef __APPLE__
    #include <sys/uio.h>
    #include <sys/time.h>
#else
    #include <sys/io.h>
    #include <time.h>
#endif
#include <dirent.h>
#include<fstream>

using namespace std;
using namespace __gnu_cxx;
void read(int &x);
void read(double &x);
int ran(int x);
char* StrToChar(string str);
void ReidGraph();
const int INF = 1000000000;

#endif //Communtity_INCLUDE_H
