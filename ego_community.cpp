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

            for(int u = 0; u < graph.n_; u++)  if(graph.exist_vertex_[u]) {
                Graph ego_graph = BuildEgoNetwork(graph, u);
                CoreDelLine core_del_line = FindCore(ego_graph);
                if (core_del_line[core_del_line.size() - 1].k_ <= ego_core_k) {
                    graph.DeleteVertex(u);
                    ego_core_del_line[ego_core_del_line.size()-1].del_vertex_.push_back(u);
                }
            }
            next_ego_core_k = INF;
            for(int u = 0; u < graph.n_; u++)  if(graph.exist_vertex_[u]) {
               // cout<<"u="<<u<<endl;
                Graph ego_graph = BuildEgoNetwork(graph, u);
                CoreDelLine core_del_line = FindCore(ego_graph);
                next_ego_core_k = min(next_ego_core_k, core_del_line[core_del_line.size() - 1].k_ );//next_ego_core_k= min alive ego1--egon
               // cout << u << " " << core_del_line[core_del_line.size() - 1].k_<< " "<< ego_core_k << endl;

            }
        }
        ego_core_k = next_ego_core_k;
    }
    return ego_core_del_line;

}
inline void ModifyLinaerQueue(int u, int modify_degree,vector<int> &deg, vector<int> &bin,vector<int> &vert,vector<int>&pos){

    int du = deg[u];
    int pu = pos[u];
    int pw = bin[modify_degree + 1];
    int w = vert[pw];
    if(u != w){
        pos[u] = pw;
        pos[w] = pu;
        vert[pu] = w;
        vert[pw] = u;
    }
    bin[du]++;
    deg[u] = modify_degree;
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
//#ifdef DEBUG
//    LOG2("ego", ego + 1)
//    for(int i = 0; i < core.n_;i++){
//        LOG2(core.vertex_[i] + 1, core.deg_[i])
//    }
//    LOG("egoend")
//
//#endif

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
int SingleEgoCore(Graph &graph,int ego, int hope_k, CoreGraph &core, CoreBucket &core_bucket){
    for(int &st = core_bucket.start_; st < core.n_ ; st++){
        int v = core_bucket.vert_[st];
        int deg_v = core.deg_[v];

        if(core.exsit_vertex_[v] == 0) {
            ModifyLinaerQueue(v , 0, core.deg_, core_bucket.bin_, core_bucket.vert_, core_bucket.pos_); //may be wrong if deg_v == 0
            continue;// it has been deleted early.
        }

        //LOG3("Deg_v", core.vertex_[v] + 1, core.deg_[v])

        if(deg_v >= hope_k){
            if(hope_k == 0){
                LOG3("fuck", core.vertex_[v] + 1, core.deg_[v])
            }
            core.k_ = deg_v;
            return 1;
        }//suit new core
       // LOG3("ego",ego + 1,core.k_)
        ModifyLinaerQueue(v , 0, core.deg_, core_bucket.bin_, core_bucket.vert_, core_bucket.pos_); //may be wrong if deg_v == 0
        core.exsit_vertex_[v] = 0;
        Neighbor neighbor_ego_v = graph.NeighborTwoVertex(ego, core.vertex_[v]);
        for(auto ori_u:neighbor_ego_v){
            int u = core.core_vertex_id_[ori_u];
            if(core.deg_[u] > deg_v){
                ModifyLinaerQueue(u, core.deg_[u] - 1, core.deg_, core_bucket.bin_, core_bucket.vert_, core_bucket.pos_);
            }
        }
    }
    return 0;
}
void DeleteVertexInEgoCore(Graph &graph, int u, int v, CoreGraph &core, CoreBucket &core_bucket){
    assert(core.core_vertex_id_.find(v) != core.core_vertex_id_.end());
    if(core.exsit_vertex_[core.core_vertex_id_[v]] == 0) return;
    core.exsit_vertex_[core.core_vertex_id_[v]] = 0;

    Neighbor neighbor_u_v = graph.NeighborTwoVertex(u, v);
    LOG3("delinego",u + 1 , v + 1);
    LOG2("core_vertex_id", core.core_vertex_id_[v] + 1)
    for(auto ori_w: neighbor_u_v){
        int w = core.core_vertex_id_[ori_w];
       // LOG4("decdeg",ori_w + 1, core_bucket.start_, core.deg_[core_bucket.vert_[core_bucket.start_]])
        if(core.exsit_vertex_[w] == 0)continue;
        ModifyLinaerQueue(w, core.deg_[w] - 1, core.deg_, core_bucket.bin_, core_bucket.vert_, core_bucket.pos_);
       // LOG4("decdeg_end",ori_w + 1,core_bucket.start_,  core.deg_[core_bucket.vert_[core_bucket.start_]])
    }
    core.k_ = -1;
    SingleEgoCore(graph, u, core.k_ + 1, core, core_bucket); // get new ego core'k
    LOG3("new_ego_core_k", u + 1, core.k_)
//    for(int v = 0; v < core.n_; v++)LOG2(core.vertex_[core_bucket.vert_[v]] + 1, core.deg_[core_bucket.vert_[v]])
//    for(int v = 0; v < core.n_; v++) if(core.exsit_vertex_[v]){
//            for(int u = v + 1; u < core.n_; u++) if(core.exsit_vertex_[u]){
//                int ori_v = core.vertex_[v] , ori_u =core.vertex_[u];
//                if((graph.exist_edge_.find(EdgeId(ori_u, ori_v)) != graph.exist_edge_.end()) && graph.exist_edge_[EdgeId(ori_u, ori_v)]) {
//                    LOG2(ori_u+1, ori_v+1)
//                }
//            }
//    }
}
EgoCoreDelLine EgoCore(Graph graph){
    EgoCoreDelLine ego_core_del_line;
    int n = graph.n_;

    // init each ego core
    priority_queue<EgoCoreOfVertex, vector<EgoCoreOfVertex>, greater<EgoCoreOfVertex> > ego_core_priority_queue;

    vector<CoreBucket> core_bucket(n + 10);
    vector<CoreGraph> core(n + 10);
    for(int v = 0; v < n; v++){
        SingleEgoCoreInit(graph, v, core[v], core_bucket[v]);
        ego_core_priority_queue.push(EgoCoreOfVertex(core[v].k_, v));
    }

    // start count  largest ego core'k

    while(!ego_core_priority_queue.empty()) {
        EgoCoreOfVertex ego_core_of_vertex = ego_core_priority_queue.top();
        int v = ego_core_of_vertex.vertex_, k = ego_core_of_vertex.k_;
        ego_core_priority_queue.pop();
        if(k != core[v].k_ || graph.exist_vertex_[v] == 0){
            // it is not real value in prioriy_queue
            //LOG3(v+1, core[v].k_, k)
            continue;
        }
        LOG3("start", v + 1, k)
        int inc_core_k_flag = SingleEgoCore(graph, v, core[v].k_ + 1, core[v], core_bucket[v]); // inc ego core'k
        if(inc_core_k_flag == 1){
            //ego core'k doesn't achieve maximum
            LOG3("inc",v + 1, core[v].k_)
            ego_core_priority_queue.push(EgoCoreOfVertex(core[v].k_, v));

        }else{
            //this ego core'k achieves maximum but it is smallest in all ego core.
            LOG3("Del_origraph",v + 1, core[v].k_)

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
#ifdef DEBUG_ASSERT
                if(graph.exist_edge_[EdgeId(u, v)] == 0){
                    assert(graph.exist_vertex_[u] == 0);
                    continue;
                }
#endif
                DeleteVertexInEgoCore(graph, u, v, core[u], core_bucket[u]);
                ego_core_priority_queue.push(EgoCoreOfVertex(core[u].k_, u));
            }
            graph.DeleteVertex(v);
        }
    }
    LOG("end")
    return ego_core_del_line;



}