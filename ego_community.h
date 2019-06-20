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
class EgoCoreOfVertex{
public:
    EgoCoreOfVertex(const int k, const int vertex){k_ = k, vertex_ = vertex;}
    int k_,vertex_;
    bool operator <(EgoCoreOfVertex element) const  {  return k_ < element.k_ ||(k_ == element.k_ && vertex_ > element.vertex_); }
    bool operator >(EgoCoreOfVertex element) const  {  return k_ > element.k_ ||(k_ == element.k_ && vertex_ > element.vertex_); }
};
class EgoVertex{
public:
    int v_, ego_;
    EgoVertex(const int v, const int ego){v_ = v, ego_ = ego;}
};

typedef CoreDelVertex EgoCoreDelVertex;
typedef CoreDelLine EgoCoreDelLine;
EgoCoreDelLine EgoCoreBaseline(Graph graph);
EgoCoreDelLine EgoCore(Graph graph);
EgoCoreDelLine EgoCoreFast(Graph graph);
#endif //COMMUNTITY_EGO_COMMUNITY_H
