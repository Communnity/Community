//
// Created by Jiyang Gong on 2019-05-15.
//

#include "ks_core.h"
int FindMinKSDegreeVertex(Graph graph,int para_k_s){



    vector<int> exsit_edge(graph.n_);

    for(int u = 0; u < graph.n_; u++){
        int sup_u = 0;
        for(auto edge_u:graph.edge_[u]) {
            exsit_edge[edge_u.point_] = 1;
        }
        for(auto edge_u:graph.edge_[u]) {
            int v = edge_u.point_;
            for(auto edge_v:graph.edge_[v]){
                int w = edge_v.point_;
                if(exsit_edge[w])
                    sup_u++;
            }
        }
        for(auto edge_u:graph.edge_[u]) {
            exsit_edge[edge_u.point_] = 0;
        }
        if(sup_u < para_k_s){
            return u;
        }

    }
    return -1;

}
void ComputerKSCore(Graph graph, int para_k, int para_s){
    int para_k_s = max(para_k, para_s + 1);
    bool min_ks_degree = 1, no_del_vertex = 1;
    while (min_ks_degree && no_del_vertex){
        int u = FindMinKSDegreeVertex(graph, para_k_s);
        if(u == -1)

    }