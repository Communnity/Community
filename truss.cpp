
// Created by Jiyang Gong on 2019-05-13.
//

#include "truss.h"


vector<SupEdge> ComputerSup(Graph g){
    vector<int> deg;
    for(int v = 0; v < g.n_; v++){
        deg.push_back(g.edge_[v].size());
    }
    vector<SupEdge> vec_sup_edge;
    unordered_map<int, int> exist_edge;

    for(int v = 0; v < g.n_; v++){
        for(auto edge_v:g.edge_[v]) {
            int u = edge_v.point_;
            if(v > u) continue;// (v,u) and (u,v) only count once
            SupEdge sup_edge(v, u, 0);
            //cout << sup_edge.v_<< " " << sup_edge.u_<<endl;
            vec_sup_edge.push_back(sup_edge);
            exist_edge[v * 1LL* g.n_ + u] = 1;
        }
    }


    for(auto &sup_edge:vec_sup_edge){

       int triangle_v = sup_edge.v_, triangle_u = sup_edge.u_;
       if(deg[triangle_v] > deg[triangle_u]) swap(triangle_v,triangle_u);
       for(auto edge_triangle_u:g.edge_[triangle_u]){
            int triangle_w = edge_triangle_u.point_;
            int id_vw;
            if(triangle_v < triangle_w)id_vw = triangle_v * g.n_ + triangle_w;
            else id_vw = triangle_w * g.n_ + triangle_v;
            if(exist_edge.find(id_vw) != exist_edge.end()) {
                sup_edge.sup_+=1;
            }
       }
    }
    //for(auto sup_edge:vec_sup_edge)cout<<sup_edge.v_<<" "<<sup_edge.u_<<" "<<sup_edge.sup_<<endl;
    return vec_sup_edge;

}
void ReorderSupEdge(int e1 , vector<SupEdge> &vec_sup_edge, vector<int> &pos, vector<int> &bin, vector<int> &edge){

    int sup_e1 = vec_sup_edge[e1].sup_;
    int pe1 = pos[e1];
    int pe2 = bin[sup_e1];
    int e2 = edge[pe2];
    if( e1 != e2){
        pos[e1] = pe2;
        pos[e2] = pe1;
        edge[pe1] = e2;
        edge[pe2] = e1;
    }
    bin[sup_e1]++;vec_sup_edge[e1].sup_--;

}
Truss FindTruss(Graph g){
    Truss truss;
    int max_sup = 0;
    int n = g.n_;
    int m = g.CalEdgeNum();
    vector<SupEdge> vec_sup_edge = ComputerSup(g);
    vector<int>  bin(m + 10);  // bin[x] = (sup_edge = x)'start
    vector<int>  pos(m + 10);  // sup_edge'pos in edge array
    vector<int>  edge(m + 10); // edge order by sup

    assert(m == vec_sup_edge.size()*2);

    for(auto sup_edge:vec_sup_edge) {
        max_sup = max(max_sup, sup_edge.sup_);
    }

    for(int d = 0; d <= max_sup; d++) bin[d] = 0;
    for(auto sup_edge:vec_sup_edge) bin[sup_edge.sup_]++;
    int start = 0;
    for(int d = 0; d <= max_sup; d++){
        int num = bin[d];
        bin[d] = start;
        start += num;
    }
    cout << max_sup<<" "<<vec_sup_edge.size()<<endl;
    for(int i = 0; i < vec_sup_edge.size(); i++){
        pos[i] = bin[vec_sup_edge[i].sup_];
        edge[pos[i]] = i;
        bin[vec_sup_edge[i].sup_]++;
    }

    for(int i = max_sup; i>=1; i--){
        bin[i] = bin[i-1];
    }
    bin[0] = 1;

    unordered_map<long long, int> exist_edge;

    for(int i = 0;i < vec_sup_edge.size(); i++){
        int u = vec_sup_edge[i].u_, v = vec_sup_edge[i].v_;
        assert(v < u);
        exist_edge[v * 1LL* g.n_ + u] = i;
    }

    for(int st = 0; st < vec_sup_edge.size(); st++){
        int v = vec_sup_edge[edge[st]].v_;
        int u = vec_sup_edge[edge[st]].u_;
        assert(v < u);
        truss.push_back(vec_sup_edge[edge[st]]);
        int id_vu = exist_edge[v * g.n_ +u];
        for(auto edge_v:g.edge_[v]){
            int w = edge_v.point_ ;
            if(w == u) continue;
            if(exist_edge.find(u *g.n_+w) == exist_edge.end()) continue;// exist v,u,w triangle
            int id_vw;
            if(v < w) id_vw = exist_edge[v * 1LL* g.n_ +w]; else id_vw = exist_edge[w * 1LL* g.n_ +v];
            if(vec_sup_edge[id_vw].sup_>vec_sup_edge[id_vu].sup_) ReorderSupEdge(id_vw, vec_sup_edge,pos,bin,edge);
            int id_uw;
            if(u < w) id_uw = exist_edge[u * 1LL* g.n_ +w]; else id_uw = exist_edge[w * 1LL* g.n_ +u];
            if(vec_sup_edge[id_uw].sup_>vec_sup_edge[id_vu].sup_) ReorderSupEdge(id_uw, vec_sup_edge,pos,bin,edge);
        }
    }

    return truss;
}