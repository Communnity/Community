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
    int ego_core_k = INF;
    for(int u = 0; u < graph.n_; u++)  if(graph.exist_vertex_[u]) {
        Graph ego_graph = BuildEgoNetwork(graph, u);
        CoreDelLine core_del_line = FindCore(ego_graph);
        ego_core_k  = min(ego_core_k, core_del_line[core_del_line.size() - 1].k_);
    }
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
                    graph.DeleteVertex(u);
                    ego_core_del_line[ego_core_del_line.size()-1].del_vertex_.push_back(u);
                }
            }
        }
        ego_core_k = next_ego_core_k;
    }
    return ego_core_del_line;

}
void Modify(CoreGraph &core, int u, int modify_degree,vector<int> &deg, vector<int> &bin,vector<int> &vert,vector<int>&pos){

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

void SingleEgoCoreInit(Graph &graph, int ego, CoreGraph &core, CoreBucket &core_bucket) {


    int n = graph.edge_[ego].size();
    if (n == 0) {
        core.k_ = -1;
        return;
    }
    core_bucket.bin_.resize(n + 10);
    core_bucket.pos_.resize(n + 10);
    core_bucket.vert_.resize(n + 10);
    core.k_ = INF;
    core.n_ = 0;
    for (auto edge_v:graph.edge_[ego]) {
        int u = edge_v.point_;
        int use_u = u, use_v = ego;
        int deg_u = graph.NeighborTwoVertex(u, ego).size();
        core.vertex_.push_back(u);
        core.exsit_vertex_.push_back(1);
        core.deg_.push_back(deg_u);
        core.core_vertex_id_[u] = core.n_;
        core.n_++;
        core.k_ = min(core.k_, deg_u);

    }
    LOG2("ego", ego + 1)
    for(int i = 0; i < core.n_;i++){
        LOG2(core.vertex_[i] + 1, core.deg_[i])
    }
    LOG("egoend")
    int md = 0;
    for (auto deg:core.deg_) md = max(md, deg);
    for (int d = 0; d <= md; d++) core_bucket.bin_[d] = 0;
    for (auto deg:core.deg_) core_bucket.bin_[deg]++;
    int start = 0;
    for (int d = 0; d <= md; d++) {
        int num = core_bucket.bin_[d];
        core_bucket.bin_[d] = start;
        start += num;
    }
    for (int v = 0; v < core.n_; v++) {
        core_bucket.pos_[v] = core_bucket.bin_[core.deg_[v]];
        core_bucket.vert_[core_bucket.pos_[v]] = v;
        core_bucket.bin_[core.deg_[v]]++;
    }
//    cout<<"md="<<md<<endl;
    for (int d = md; d >= 1; d--) {
        core_bucket.bin_[d] = core_bucket.bin_[d - 1];
    }
    core_bucket.bin_[0] = 0;
    core_bucket.start_ = 0;
}
int SingleEgoCore(Graph &graph,int ego, CoreGraph &core, CoreBucket &core_bucket){
    for(int &st = core_bucket.start_; st < core.n_ ; st++){
        int v = core_bucket.vert_[st];
        int deg_v = core.deg_[v];
        if(core.exsit_vertex_[v] == 0) continue;// it has been deleted early.
        LOG2(core.vertex_[v] + 1, core.exsit_vertex_[v])
        LOG3("Deg_v", core.vertex_[v] + 1, core.deg_[v])
        //Modify(core, v , 0, core.deg_, core_bucket.bin_, core_bucket.vert_, core_bucket.pos_); may be wrong if deg_v == 0

        if(deg_v > core.k_){
            core.k_ = deg_v;
            return 1;
        }//suit new core
        LOG3("ego",ego + 1,core.k_)
        core.exsit_vertex_[v] = 0;
        Neighbor neighbor_ego_v = graph.NeighborTwoVertex(ego, core.vertex_[v]);
        for(auto ori_u:neighbor_ego_v){
            int u = core.core_vertex_id_[ori_u];
            if(core.deg_[u] > deg_v){
                Modify(core, u, core.deg_[u] - 1, core.deg_, core_bucket.bin_, core_bucket.vert_, core_bucket.pos_);
            }
        }
    }
    return 0;
}
void DeleteVertexInEgoCore(Graph &graph, int u, int v, CoreGraph &core, CoreBucket &core_bucket){
    if(core.exsit_vertex_[core.core_vertex_id_[v]] == 0) return;
    LOG3("delinego",u + 1 , v + 1);
    core.exsit_vertex_[core.core_vertex_id_[v]] = 0;
    Neighbor neighbor_u_v = graph.NeighborTwoVertex(u, v);
    LOG3("delinego",u + 1 , v + 1);
    for(auto ori_w: neighbor_u_v){
        int w = core.core_vertex_id_[ori_w];
        if(core.exsit_vertex_[w] == 0)continue;
        Modify(core, w, core.deg_[w] - 1, core.deg_, core_bucket.bin_, core_bucket.vert_, core_bucket.pos_);
    }
    core.k_ = core.deg_[core_bucket.vert_[core_bucket.start_]];
}
EgoCoreDelLine EgoCore(Graph graph){
    EgoCoreDelLine ego_core_del_line;
    int n = graph.n_;

    priority_queue<EgoCoreOfVertex, vector<EgoCoreOfVertex>, greater<EgoCoreOfVertex> > ego_core_priority_queue;

    vector<CoreBucket> core_bucket(n + 10);
    vector<CoreGraph> core(n + 10);
    for(int v = 0; v < n; v++){
        SingleEgoCoreInit(graph, v, core[v], core_bucket[v]);
        LOG2(core[v].k_, v)
        ego_core_priority_queue.push(EgoCoreOfVertex(core[v].k_, v));
    }


    while(!ego_core_priority_queue.empty()) {
        EgoCoreOfVertex ego_core_of_vertex = ego_core_priority_queue.top();
        int v = ego_core_of_vertex.vertex_, k = ego_core_of_vertex.k_;
        ego_core_priority_queue.pop();
        if(k != core[v].k_ || graph.exist_vertex_[v] == 0){
            // it is not real value in prioriy_queue
            LOG2(core[v].k_, v)
            LOG2(k, v)
            continue;
        }
        LOG2("start", v + 1)
        int inc_core_k_flag = SingleEgoCore(graph,v, core[v], core_bucket[v]);
        if(inc_core_k_flag == 1){
            LOG2(core[v].k_, v + 1)
            ego_core_priority_queue.push(EgoCoreOfVertex(core[v].k_, v));

        }else{
            LOG2("Del_origraph",v + 1)

            if(ego_core_del_line.size() == 0 || core[v].k_ + 1> ego_core_del_line[ego_core_del_line.size()-1].k_){
                CoreDelVertex del_vertex;
                del_vertex.k_ = core[v].k_ + 1;
                del_vertex.del_vertex_.push_back(v);
                ego_core_del_line.push_back(del_vertex);
            }else{
                ego_core_del_line[ego_core_del_line.size()-1].del_vertex_.push_back(v);
            }
            assert(graph.exist_vertex_[v] == 1);
            for(auto edge_v:graph.edge_[v]) {
                int u = edge_v.point_;
                if(graph.exist_edge_[EdgeId(u, v)] == 0){
                    assert(graph.exist_vertex_[u] == 0);
                    continue;
                }
                DeleteVertexInEgoCore(graph, u, v, core[u], core_bucket[u]);
                ego_core_priority_queue.push(EgoCoreOfVertex(core[u].k_, u));
            }
            graph.DeleteVertex(v);
        }
    }
    LOG("end")
    return ego_core_del_line;



}