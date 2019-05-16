//
// Created by Jiyang Gong on 2019-03-26.
//

#include "cores.h"


CoresDelLine FindCores(Graph g){
    int n = g.n_;
    int md = 0;
    vector<int>  bin(g.n_ + 10);
    vector<int>  pos(g.n_ + 10);
    vector<int>  vert(g.n_ + 10);

    vector<int> deg;
    for(int v = 0; v < n; v++) deg.push_back(g.edge_[v].size());
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

    for(int d = md; d>=1; d--){
        bin[d] = bin[d-1];
    }
    bin[0] = 1;
    CoresDelLine cores_del_line;
    //cout<<"n="<<n<<endl;
    for(int st = 0; st < n; st++){
        int v = vert[st];
        vector<Edge> &edge_v  = g.edge_[v];
       // cout<<st<<" "<<v<<" "<<deg[v]<< endl;
        if(cores_del_line.size()==0 || deg[v]>cores_del_line[cores_del_line.size()-1].k_){
            CoresDelVertex del_vertex;
            del_vertex.k_ = deg[v];
            del_vertex.del_vertex_.push_back(v);
            cores_del_line.push_back(del_vertex);
        }else{
            cores_del_line[cores_del_line.size()-1].del_vertex_.push_back(v);
        }
        for(int i = 0; i < edge_v.size(); i++){
            int u = edge_v[i].point_ , w = edge_v[i].weight_;

            if(deg[u]>deg[v]){
                int du = deg[u];
                int pu = pos[u];
                int pw = bin[du];
                int w = vert[pw];
                if(u != w){
                    pos[u] = pw;
                    pos[w] = pu;
                    vert[pu] = w;
                    vert[pw] = u;
                }
                bin[du]++;deg[u]--;
            }
        }
    }
    for(int i = 0; i < cores_del_line.size(); i++){
        printf("%d ",cores_del_line[i].k_);
        for(int j = 0; j < cores_del_line[i].del_vertex_.size(); j++){
            printf("%d ",cores_del_line[i].del_vertex_[j] + 1);
        }
        printf("\n");
    }
    return cores_del_line;
}
bool CheckCoresDegree(int n,int cores_k, vector<int> &deg){
    for(int i = 0;i < n; i++) if(deg[i] >= 0){
            if(deg[i] < cores_k) return false;
        }
    return true;
}
bool CheckCoresDelLine(Graph graph, CoresDelLine cores_del_line){

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

Cores FindLeastColorCores(Graph graph, NeedColorList need_color_vertex_list, CoresDelLine cores_del_line){

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


