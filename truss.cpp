
// Created by Jiyang Gong on 2019-05-13.
//

#include "truss.h"

Graph BuildTruss(Truss_sup truss_sup, int truss_k){
    Graph truss;
    truss.n_ = truss.m_ = 0;
    for(int  i = 0; i < truss_sup.size(); i++){
        int u = truss_sup[i].u_, v = truss_sup[i].v_ , sup = truss_sup[i].sup_;
        if(sup < truss_k) continue;
        truss.n_ = max(truss.n_, max(u,v) + 1);
    }

    truss.Init();// must give n to init edge[1]....edge[n]

    for(int  i = 0; i < truss_sup.size(); i++){
        int u = truss_sup[i].u_, v = truss_sup[i].v_ , sup = truss_sup[i].sup_;
        if(sup < truss_k) continue;

        truss.exist_vertex_[u]  = truss.exist_vertex_[v] = 1;
        truss.edge_[u].push_back(v);
        truss.edge_[v].push_back(u);
        truss.exist_edge_[EdgeId(u,v)] = 1;

        truss.m_++;
    }



    truss  = truss.RenewGraphwithReid();
    return truss;
}
Truss_sup ComputerSup(Graph graph){
    vector<int> deg;
    for(int v = 0; v < graph.n_; v++){
        deg.push_back(graph.edge_[v].size());
    }
    vector<SupEdge> vec_sup_edge;


    for(int v = 0; v < graph.n_; v++){
        for(auto edge_v:graph.edge_[v]) {
            int u = edge_v;
            if(v > u) continue;// (v,u) and (u,v) only count once
            SupEdge sup_edge(v, u, 0);
            //cout << sup_edge.v_<< " " << sup_edge.u_<<endl;
            vec_sup_edge.push_back(sup_edge);
        }
    }
    for(auto &sup_edge:vec_sup_edge){
       int triangle_v = sup_edge.v_, triangle_u = sup_edge.u_;
       if(deg[triangle_u] > deg[triangle_v]) swap(triangle_v,triangle_u);
       for(auto edge_triangle_u:graph.edge_[triangle_u]){
            int triangle_w = edge_triangle_u;
            if(graph.exist_edge_.find(EdgeId(triangle_v, triangle_w)) != graph.exist_edge_.end()) {
                sup_edge.sup_+=1;
            }
       }
    }
    //for(auto sup_edge:vec_sup_edge)cout<<sup_edge.v_<<" "<<sup_edge.u_<<" "<<sup_edge.sup_<<endl;
    return vec_sup_edge;
}
void ReorderSupEdge(int e1 , vector<SupEdge> &vec_sup_edge, vector<int> &pos, vector<int> &bin, vector<int> &edge){

    int sup_e1 = vec_sup_edge[e1].sup_;
    assert(sup_e1 > 0);
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
Truss_sup FindTruss(Graph graph){

    int max_sup = 0;
    int n = graph.n_;
    int m = graph.CalEdgeNum();
    vector<SupEdge> vec_sup_edge = ComputerSup(graph);
    vector<int>  bin(vec_sup_edge.size() + 10);  // bin_[x] = (sup_edge = x)'start_
    vector<int>  pos(vec_sup_edge.size() + 10);  // sup_edge'pos_ in edge array
    vector<int>  edge(vec_sup_edge.size() + 10); // edge order by sup
    Truss_sup truss;
    SupEdge empty_sup_edge;
    for(int i = 0; i < vec_sup_edge.size(); i++) truss.push_back(empty_sup_edge);

    vector<int>  deg;
    for(int v = 0; v < n; v++) deg.push_back(graph.edge_[v].size());
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
    for(int i = 0; i < vec_sup_edge.size(); i++){
        pos[i] = bin[vec_sup_edge[i].sup_];
        edge[pos[i]] = i;
        bin[vec_sup_edge[i].sup_]++;
    }

    for(int i = max_sup; i>=1; i--){
        bin[i] = bin[i-1];
    }
    bin[0] = 0;

    unordered_map<long long, int> exist_edge;

    for(int i = 0;i < vec_sup_edge.size(); i++){
        int u = vec_sup_edge[i].u_, v = vec_sup_edge[i].v_;
        assert(v < u);
        exist_edge[EdgeId(v, u)] = i;
    }
    int truss_k = 0;
    for(int st = 0; st < vec_sup_edge.size(); st++){

        int v = vec_sup_edge[edge[st]].v_;
        int u = vec_sup_edge[edge[st]].u_;

        assert(v < u);
        truss_k = max(truss_k, vec_sup_edge[edge[st]].sup_);
        truss[edge[st]] = vec_sup_edge[edge[st]];
        truss[edge[st]].sup_ = truss_k;
        if(deg[v] > deg[u]) swap(v,u);
        assert(exist_edge.find(EdgeId(v,u)) != exist_edge.end());
        assert(deg[v] > 0);
        assert(deg[u] > 0);
        for(auto edge_v:graph.edge_[v]){
            int w = edge_v;
            if(w == u) continue;
            if(exist_edge.find(EdgeId(v,w)) == exist_edge.end()) continue;
            if(exist_edge.find(EdgeId(u,w)) == exist_edge.end()) continue;// exist v,u,w triangle
            if(vec_sup_edge[exist_edge[EdgeId(v,w)]].sup_>vec_sup_edge[exist_edge[EdgeId(v,u)]].sup_){
                //cout<<EdgeId(v,w)<<" "<<vec_sup_edge[exist_edge[EdgeId(v,w)]].sup_<<endl;
                assert(st <pos[exist_edge[EdgeId(v,w)]]), ReorderSupEdge(exist_edge[EdgeId(v,w)], vec_sup_edge,pos,bin,edge);
                //cout<<EdgeId(v,w)<<" "<<vec_sup_edge[exist_edge[EdgeId(v,w)]].sup_<<endl;
            }
            if(vec_sup_edge[exist_edge[EdgeId(u,w)]].sup_>vec_sup_edge[exist_edge[EdgeId(v,u)]].sup_){
                assert(st < pos[exist_edge[EdgeId(u,w)]]),ReorderSupEdge(exist_edge[EdgeId(u,w)], vec_sup_edge,pos,bin,edge);
            }
        }

        exist_edge.erase(EdgeId(v,u)); //（v,u) should be delete in graph to count triangle rightly.
        deg[v]--;
        deg[u]--;
    }
    cout << "Finish "<<__FUNCTION__<<endl;
    return truss;
}