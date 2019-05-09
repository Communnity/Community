//
// Created by Jiyang Gong on 2019-04-03.
//

#include "condition.h"

Color::Color(int id,int number){
    id_ = id;
    number_ = number;
}

NeedColorList InputNeedColorList(string file_str){
    FILE *fp = NULL;
    char file_s[100] = "";
    strcpy(file_s, file_str.c_str());
    fp = fopen(file_s,"r");
    int nk;
    fscanf(fp,"%d",&nk);
    NeedColorList need_color_vertex_list;
    for(int i = 0; i < nk; i++){
        int id, number;
        fscanf(fp,"%d %d",&id,&number);
        assert(id > 0);
        assert(number > 0);
        need_color_vertex_list.push_back(Color(id,number));
    }
    return need_color_vertex_list;

}
