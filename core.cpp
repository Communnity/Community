//
// Created by Jiyang Gong on 2019-03-26.
//

#include "core.h"
Graph BuildCorewithIndex(Graph graph, CoreDelLine core_del_line, int core_k){

    Graph core;
    core.n_ = core.m_ = 0;
    core.n_ = graph.n_;
    int s = 0;
    for(int i = 0; i < core.n_; i++) core.exist_vertex_[i] = 0;
    for(int i = 0;i < core_del_line.size(); i++){
        if(core_del_line[i].k_ < core_k) continue;

        for(auto v:core_del_line[i].del_vertex_){
            core.exist_vertex_[v] = 1;
            s++;
        }
    }

    core.Init();//must to give n to init edge list;
    for(int u  = 0; u < graph.n_; u++) if(core.exist_vertex_[u]){
        for(auto edge_u:graph.edge_[u]){
            int v = edge_u.point_;
            if(core.exist_vertex_[v]){
                if(v < u) continue;
                core.edge_[u].push_back(Edge(v,0));
                core.edge_[v].push_back(Edge(u,0));
                assert(core.exist_edge_[EdgeId(u,v)] == 0);
                core.exist_edge_[EdgeId(u,v)] = 1;
                core.m_++;
            }
        }
    }
    assert(core.m_ ==  core.exist_edge_.size());
    core = core.RenewGraphwithReid();

    return core;
}
void Modify(int u, int modify_degree,vector<int> &deg, vector<int> &bin,vector<int> &vert,vector<int>&pos){

    int du = deg[u];
    int pu = pos[u];
    int pw = bin[modify_degree+1];
    int w = vert[pw];
    if(u != w){
        pos[u] = pw;
        pos[w] = pu;
        vert[pu] = w;
        vert[pw] = u;
    }
    bin[du]++;
    deg[u]=modify_degree;
}

CoreDelLine FindCore(Graph &graph){
    CoreDelLine core_del_line;
    int n = graph.n_;
    if(n == 0){
        CoreDelVertex del_vertex;
        del_vertex.k_ = -1;
        core_del_line.push_back(del_vertex);
        return core_del_line;
    }
    int md = 0;
    vector<int>  bin(graph.n_ + 10);
    vector<int>  pos(graph.n_ + 10);
    vector<int>  vert(graph.n_ + 10);

    vector<int> deg;
    for(int v = 0; v < n; v++) deg.push_back(graph.edge_[v].size());
    for(auto deg_v:deg)
        md = max(md, deg_v);

    for(int d = 0; d <= md; d++) bin[d] = 0;
    for(int v = 0; v < n; v++) bin[deg[v]]++;
    int start = 0;
    for(int d = 0; d <= md; d++){
        int num = bin[d];
        bin[d] = start;
        start += num;
    }
    for(int v = 0; v < n; v++){
        pos[v] = bin[deg[v]];
        vert[pos[v]] = v;
        bin[deg[v]]++;
    }
//    cout<<"md="<<md<<endl;
    for(int d = md; d>=1; d--){
        bin[d] = bin[d-1];
    }
    bin[0] = 0;

    //cout<<"n="<<n<<endl;
    for(int st = 0; st < n; st++){
        int v = vert[st], deg_v = deg[v];
        // Modify(v,0,deg,bin,vert,pos); may be wrong if deg_v == 0
        // cout<<st<<" "<<v<<" "<<deg[v]<< endl;
        if(core_del_line.size()==0 || deg_v>core_del_line[core_del_line.size()-1].k_){
            CoreDelVertex del_vertex;
            del_vertex.k_ = deg_v;
            del_vertex.del_vertex_.push_back(v);
            core_del_line.push_back(del_vertex);
        }else{
            core_del_line[core_del_line.size()-1].del_vertex_.push_back(v);
        }
        vector<Edge> edge_v  = graph.edge_[v];
        for(int i = 0; i < edge_v.size(); i++){
            int u = edge_v[i].point_;

            if(deg[u]>deg_v){
                Modify(u,deg[u]-1,deg,bin,vert,pos);
            }
        }
    }
//    for(int i = 0; i < core_del_line.size(); i++){
//        if(i < core_del_line.size() - 1) continue;
//        printf("Core'K=%d Size=%d",core_del_line[i].k_, core_del_line[i].del_vertex_.size());
//        for(int j = 0; j < core_del_line[i].del_vertex_.size(); j++){
//            printf("%d %d\n",core_del_line[i].del_vertex_[j] + 1,deg[core_del_line[i].del_vertex_[j]]);
//        }
//        printf("\n");
//    }
    //
    //  cout << "Finish "<<__FUNCTION__<<endl;
    return core_del_line;
}

Graph FindKCore(Graph graph, int k_degree){
    CoreDelLine core_del_line = FindCore(graph);
    for(int i = 0; i < core_del_line.size(); i++){
        if(core_del_line[i].k_ >= k_degree){
            break;
        }
        for(int j = 0; j < core_del_line[i].del_vertex_.size(); j++){
            int u = core_del_line[i].del_vertex_[j];
            graph.exist_vertex_[u] = 0;
        }
    }
    graph = graph.RenewGraph();
    return graph;

}
bool CheckCoresDegree(int n,int cores_k, vector<int> &deg){
    for(int i = 0;i < n; i++) if(deg[i] >= 0){
            if(deg[i] < cores_k) return false;
        }
    return true;
}
bool CheckCoresDelLine(Graph graph, CoreDelLine cores_del_line){

    int n = graph.n_;
    int m = graph.m_;
    vector<int> deg;
    for(int i = 0; i < n; i++){
        deg.push_back(graph.edge_[i].size());
    }

    for(int i = 0; i < cores_del_line.size();i++){
        int cores_k = cores_del_line[i].k_;
        for(int j = 0; j  < cores_del_line[i].del_vertex_.size(); j++){
            int v = cores_del_line[i].del_vertex_[j];
            deg[v] = - 1;
            for(int k  = 0; k < graph.edge_[v].size(); k++){
                int u = graph.edge_[v][k].point_;
                deg[u]--;
            }
        }
        if(!CheckCoresDegree(n,cores_k,deg)) return false;

    }

    return true;
}

Cores FindLeastColorCores(Graph graph, NeedColorList need_color_vertex_list, CoreDelLine cores_del_line){

    int n = graph.n_;
    int m = graph.m_;
    Cores cores;
    cores.k_ = 0;
    int max_attribute = 0;

    for(int v = 0; v < n; v++) max_attribute = max(max_attribute, graph.vertex_[v].attribute_);
    vector<int> color(max_attribute),need_color_vertex(max_attribute),vertex_delete(n);

    for(int v = 0; v < n; v++) color[graph.vertex_[v].attribute_]++;
    for(int i = 0;i < need_color_vertex_list.size(); i++){
        need_color_vertex[need_color_vertex_list[i].id_] += need_color_vertex_list[i].number_;
    }
    for(int i = 0; i < max_attribute;i++) if(color[i] < need_color_vertex[i]){
        return cores;
    }


    for(int i = 0; i < cores_del_line.size()-1;i++){

        for(int j = 0; j  < cores_del_line[i].del_vertex_.size(); j++){
            int v = cores_del_line[i].del_vertex_[j];
            color[graph.vertex_[v].attribute_]--;
            if(color[graph.vertex_[v].attribute_] < need_color_vertex[graph.vertex_[v].attribute_]){
                cores.k_ = cores_del_line[i].k_;
                for(int i = 0; i < n; ++i) if(vertex_delete[i] == 0)
                    cores.vertex_.push_back(i);
                return cores;
            }
        }
        for(int j = 0; j  < cores_del_line[i].del_vertex_.size(); j++) {
            int v = cores_del_line[i].del_vertex_[j];
            vertex_delete[v] = 1;
        }
    }
    cores.k_ = cores_del_line[cores_del_line.size()-1].k_;
    for(int i = 0; i < n; ++i) if(vertex_delete[i] == 0)
            cores.vertex_.push_back(i);
    return cores;
}


