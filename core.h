
// Created by Jiyang Gong on 2019-03-26.
//

#ifndef Communtity_CORES_H
#define Communtity_CORES_H
#include "include.h"
#include "graph.h"
#include "condition.h"

class CoreDelVertex{
    public:
        int  k_;
        vector<int> del_vertex_;
};
typedef vector<CoreDelVertex> CoreDelLine;

class Cores{
    public:
        int k_;
        vector<int> vertex_;
};
CoreDelLine  FindCore(Graph &graph);
Graph FindKCore(Graph graph);
bool CheckCoresDelLine(Graph graph, CoreDelLine cores_del_line);
Cores FindLeastColorCores(Graph graph, NeedColorList need_color_vertex_list, CoreDelLine cores_del_line);
Graph BuildCorewithIndex(Graph graph, CoreDelLine core_del_line, int core_k);
#endif //Communtity_CORES_H
