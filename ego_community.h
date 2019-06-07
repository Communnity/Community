//
// Created by Jiyang Gong on 2019-06-04.
//

#ifndef COMMUNTITY_EGO_COMMUNITY_H
#define COMMUNTITY_EGO_COMMUNITY_H
#include "include.h"
#include "graph.h"
#include "core.h"
//class EgoCoreDelVertex{
////    public:
////        int  k_;
////        vector<int> del_vertex_;
////};
typedef CoreDelVertex EgoCoreDelVertex;
typedef CoreDelLine EgoCoreDelLine;
EgoCoreDelLine EgoCoreBaseline(Graph graph);

#endif //COMMUNTITY_EGO_COMMUNITY_H
