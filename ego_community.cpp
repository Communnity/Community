//
// Created by Jiyang Gong on 2019-06-04.
////

#include "ego_community.h"
Graph BuildEgoNetwork(Graph graph,int ego){
    for(int u = 0; u < graph.n_; u++){
        if(u == ego) continue;
        if(graph.exist_edge_[EdgeId(ego, u)]) continue;
        graph.exist_vertex_[u] = 0;
    }
    return graph.RenewGraphwithReid();
}
EgoCoreDelLine EgoCoreBaseline(Graph graph){
    int ego_core_k = 0;

    EgoCoreDelLine ego_core_del_line;
    while(!graph.Empty()){
        int next_ego_core_k = -INF;
        EgoCoreDelVertex ego_core_del_vertex;
        ego_core_del_vertex.k_ = ego_core_k;
       // cout<<ego_core_k<<endl;
        ego_core_del_line.push_back(ego_core_del_vertex);
        while(next_ego_core_k <= ego_core_k){
            next_ego_core_k = INF;
            for(int u = 0; u < graph.n_; u++)  if(graph.exist_vertex_[u]) {
               // cout<<"u="<<u<<endl;
                Graph ego_graph = BuildEgoNetwork(graph, u);
                CoreDelLine core_del_line = FindCore(ego_graph);
                next_ego_core_k = min(next_ego_core_k, core_del_line[core_del_line.size() - 1].k_ );//next_ego_core_k= min alive ego1--egon
               // cout << u << " " << core_del_line[core_del_line.size() - 1].k_<< " "<< ego_core_k << endl;
                if (core_del_line[core_del_line.size() - 1].k_ <= ego_core_k) {
                    graph.exist_vertex_[u] = 0;
                    ego_core_del_line[ego_core_del_line.size()-1].del_vertex_.push_back(u);
                }
            }
        }
        ego_core_k = next_ego_core_k;
    }
    return ego_core_del_line;

}