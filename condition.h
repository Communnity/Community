//
// Created by Jiyang Gong on 2019-04-03.
//

#ifndef Communtity_CONDITION_H
#define Communtity_CONDITION_H
#include "include.h"
class Color{
    public:
        int id_;
        int number_;
        Color(int id, int number);
};
typedef vector<Color> NeedColorList;

NeedColorList InputNeedColorList(string file_str);


#endif //Communtity_CONDITION_H
