
// Created by Jiyang Gong on 2019-03-26.
//

#ifndef Communtity_CORES_H
#define Communtity_CORES_H
#include "include.h"
#include "graph.h"
#include "condition.h"

class CoresDelVertex{
    public:
        int  k_;
        vector<int> del_vertex_;
};
typedef vector<CoresDelVertex> CoreDelLine;

class Cores{
    public:
        int k_;
        vector<int> vertex_;
};
CoreDelLine  FindCore(Graph graph);
bool CheckCoresDelLine(Graph graph, CoreDelLine cores_del_line);
Cores FindLeastColorCores(Graph graph, NeedColorList need_color_vertex_list, CoreDelLine cores_del_line);
#endif //Communtity_CORES_H
