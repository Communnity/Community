//
// Created by Jiyang Gong on 2019-07-05.
//

#include "clique.h"

bool CheckNonReduced(vector<int> set){
    for(int  i = 0; i < set.size() - 1; i++)
        if(set[i] > set[i + 1]) return false;
    return true;
}
vector<int> SetAnd(vector<int> set_a, vector<int> set_b){
//    assert(CheckNonReduced(set_a));
//    assert(CheckNonReduced(set_b));
//    sort(set_a.begin(), set_a.end());
//    sort(set_b.begin(), set_b.end());
    int i = 0, j = 0;
    vector<int> set_c;
    while(i < set_a.size() || j < set_b.size()) {
        if(i == set_a.size()) j++;else
        if(j == set_b.size()) i++;else
        if (set_a[i] < set_b[j]) i++;
        else if (set_a[i] > set_b[j]) j++;
        else set_c.push_back(set_a[i]), i++, j++;
    }
    return set_c;
}
vector<int> SetOr(vector<int> set_a, vector<int> set_b){

//    sort(set_a.begin(), set_a.end());
//    sort(set_b.begin(), set_b.end());
    int i = 0, j = 0;
    vector<int> set_c;
    while(i < set_a.size() || j < set_b.size()) {
        if(i == set_a.size()) set_c.push_back(set_b[j]),j++;else
        if(j == set_b.size()) set_c.push_back(set_a[i]),i++;else
        if (set_a[i] < set_b[j]) set_c.push_back(set_a[i]),i++;
        else if (set_a[i] > set_b[j]) set_c.push_back(set_b[j]),j++;
        else set_c.push_back(set_a[i]), i++, j++;
    }
    return set_c;
}
vector<int> SetSub(vector<int> set_a, vector<int> set_b){
//    assert(CheckNonReduced(set_a));
//    assert(CheckNonReduced(set_b));
//    sort(set_a.begin(), set_a.end());
//    sort(set_b.begin(), set_b.end());
    int i = 0, j = 0;
    vector<int> set_c;

    while(i < set_a.size() || j < set_b.size()) {
        LOG2(i, j)
        if(i == set_a.size()) j++;else
        if(j == set_b.size()) set_c.push_back(set_a[i]),i++;else
        if (set_a[i] < set_b[j]) set_c.push_back(set_a[i]),i++;
        else if (set_a[i] > set_b[j]) j++;
        else i++, j++;
    }
    return set_c;
}
void BasicMCE(Graph &graph, vector<int> P, vector<int> R, vector<int> X, int &mc_size){
//    if(P.size() + R.size() <= mc_size){
//        return;
//    }
    if(P.size() == 0) {
        if (SetOr(P, X).size() == 0 && R.size() > 2) {
            if (R.size() > mc_size) {
                mc_size = R.size();
            }
        }
        return;
    }
    int u = -1, u_prior = -INF;

    for(auto v:SetOr(P, X)){
        vector<int> neighbor_v;
        for(auto edge_v:graph.edge_[v]) neighbor_v.push_back(edge_v.point_);
        if(int(SetAnd(P, neighbor_v).size()) > u_prior){ // emmmmm?not int size() will wrong?
            u_prior = SetAnd(P, neighbor_v).size();
            u = v;
        }
    }
    assert(u > -1);
    vector<int> neighbor_u;
    for(auto edge_u:graph.edge_[u]) neighbor_u.push_back(edge_u.point_);

    for(auto v:SetSub(P,neighbor_u)){
        vector<int> neighbor_v;
        for(auto edge_v:graph.edge_[v]) neighbor_v.push_back(edge_v.point_);
        vector<int> set_v;set_v.push_back(v);
        BasicMCE(graph, SetAnd(P, neighbor_v), SetOr(R, set_v),SetAnd(X,neighbor_v), mc_size);
        P = SetSub(P, set_v);
        X = SetOr(X ,set_v);
    }


}


void Independent(Graph &graph){
    int cnt = 0;
    for(int u = 0; u < graph.n_; u++){
        Neighbor neighbor_u = graph.NeighborOneVertex(u);
        int clique_u = 1, size = 0;
        for(auto v:neighbor_u)
            for(auto w:neighbor_u) if(v != w) {
                cout<<u<<" "<<v<<" "<<w<<endl;
                    if (graph.exist_edge_[EdgeId(v, w)] == 0) {
                        clique_u = 0;
                        break;
                    }
                }
        if(clique_u) ++cnt,cout<<u<<endl;
    }
    printf("cnt=%d n=%d  m=%d %.5f\n",cnt, graph.n_, graph.m_, cnt*1.0/graph.n_);
}
void Clique(Graph &graph){
    int cnt = 0, m = graph.m_ * 2;
    for(int u = 0; u < graph.n_; u++){
        Neighbor neighbor_u = graph.NeighborOneVertex(u);
        int independent_u = 1, size = 2 * neighbor_u.size();
        for(auto v:neighbor_u)
            for(auto edge_v:graph.edge_[v]) {
                int w = edge_v.point_;
                if(w == u) continue;
                if(graph.exist_edge_.find(EdgeId(u, w)) == graph.exist_edge_.end())
                    size += 2;
                else size += 1;
            }
        cout << m << " " << size<< endl;
        if(m == size) ++cnt;
    }
    printf("cnt=%d n=%d  m=%d m'=%lld %.5f\n",cnt, graph.n_, graph.m_, (long long)graph.n_*(graph.n_-1)/2 - graph.m_, cnt*1.0/graph.n_);
}
