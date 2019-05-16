//
// Created by Jiyang Gong on 2019-05-15.
//

#include "ks_core.h"
int FindMinKSDegreeVertex(Graph &graph, int para_k, int para_s){


    cout <<"Start " <<  __FUNCTION__ << endl;
    for(int u = 0; u < graph.n_; u++) if(graph.exsit_vertex_[u]){
        int deg_u = 0;

        for(auto edge_u:graph.edge_[u]) {
            int v = edge_u.point_;
            if(!graph.exsit_vertex_[v]) continue;
            cout << u <<" "<<v<<" "<<graph.exsit_edge_[EdgeId(u, v)]<<endl;
            if(!graph.exsit_edge_[EdgeId(u, v)]) continue;
            int sup_u_v = 0;
            for(auto edge_v:graph.edge_[v]){
                int w = edge_v.point_;
                if(!graph.exsit_vertex_[w]) continue;
                if(!graph.exsit_edge_[EdgeId(v, w)]) continue;
                if(graph.exsit_edge_[EdgeId(u, w)] == 1)
                    sup_u_v++;
            }
            if(sup_u_v >= para_s)
                deg_u++;

        }

        if(deg_u < para_k){
            cout << "Finish FindMinKSDegreeVertex with Find u" <<endl;
            return u;
        }
    }
    cout << "Finish FindMinKSDegreeVertex with Not Find" <<endl;
    return -1;


}



bool ComputeKSCore(Graph graph, int para_k, int para_s){

    cout << "Start Compute KS Core" <<endl;

    bool min_ks_degree = 0;
    while (!(min_ks_degree == 1)){

        if(graph.Empty()){
            cout << "Finish Compute KS Core with False" <<endl;
            return false;
        }
        int u = FindMinKSDegreeVertex(graph, para_k, para_s);
        cout << u + 1 << endl;
        if(u >= 0){
            DeleteVertex(graph, u);

        } else
            min_ks_degree = 1;
    }
    cout << "Finish Compute KS Core with True" <<endl;
    return true;
}
bool ComputeKSCoreOfEgo(Graph graph, int u, int para_k, int para_s){
    for(int  v = 0; v < graph.n_; v++){
        if(u == v) continue;
        if(graph.exsit_vertex_[v] == 0) continue;
        if(graph.exsit_edge_[EdgeId(u, v)] == 1) continue;
        for(auto edge_v:graph.edge_[v]){
            int w = edge_v.point_;
            if(graph.exsit_vertex_[w] == 0){
                assert(graph.exsit_edge_[EdgeId(v, w)] == 0);
                continue;
            }
            graph.exsit_edge_[EdgeId(v, w)] = 0;
        }
    }
    cout << "Start Compute KS Core of Ego" << endl;
    return ComputeKSCore(graph, para_k, para_s);

}

bool KCliqueCommunity(Graph graph, int para_k){
    bool k_clique_community = 0;
    while(!(k_clique_community)){
        if(graph.Empty())  return false;

        k_clique_community = 1;
        for(int u = 0; u < graph.n_; u++) if(graph.exsit_vertex_[u]){
            bool compute_ks_core_of_ego = ComputeKSCoreOfEgo(graph, u, para_k - 1, para_k - 2);
            if(compute_ks_core_of_ego == 0) {
                cout<<"Delete_Vertex= "<<u<<endl;
                DeleteVertex(graph, u);
                k_clique_community = 0;
                break;
            }
        }
    }
    graph.OutputGraph();
    return true;
}
