
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
typedef vector<CoresDelVertex> CoresDelLine;

class Cores{
    public:
        int k_;
        vector<int> vertex_;
};
CoresDelLine  FindCores(Graph g);
bool CheckCoresDelLine(Graph graph, CoresDelLine cores_del_line);
Cores FindLeastColorCores(Graph graph, NeedColorList need_color_vertex_list, CoresDelLine cores_del_line);
#endif //Communtity_CORES_H
