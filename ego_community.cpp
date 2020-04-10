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
  //  assert(du >= 0);
    bin[du]++;
    deg[u] = modify_degree;
}

void SingleEgoCoreInit(Graph &graph, int ego, CoreGraph &core, CoreBucket &core_bucket) {


    int n = graph.edge_[ego].size();
    if (n == 0) {
        core.n_ = 0;
        core.k_ = -1;
        return;
    }
    core_bucket.bin_.resize(n + 10);
    core_bucket.pos_.resize(n + 10);
    core_bucket.vert_.resize(n + 10);
    core.k_ = INF;
    core.n_ = 0;
    for (auto edge_v:graph.edge_[ego]) {
        int u = edge_v;

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
            // fuck  when  0 - 》 -1  will wrong
            continue;// it has been deleted early.
        }

        //LOG3("Deg_v", core.vertex_[v] + 1, core.deg_[v])

        if(deg_v >= hope_k){
            core.k_ = deg_v;
            return 1;
        }//suit new core
       // LOG3("ego",ego + 1,core.k_)
        ModifyLinaerQueue(v , 0, core.deg_, core_bucket.bin_, core_bucket.vert_, core_bucket.pos_); //may be wrong if deg_v == 0
        core.exsit_vertex_[v] = 0;
        Neighbor neighbor_ego_v = graph.NeighborTwoVertex(ego, core.vertex_[v]);
        for(auto ori_u:neighbor_ego_v){
            int u = core.core_vertex_id_[ori_u];
            if(core.deg_[u] > deg_v)// this condition ensure  core bucket not deacrease
            {
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
    SingleEgoCore(graph, u, core.k_ + 1, core, core_bucket); // get new ego core'k = k -1 or k  need proof this core'k is right in this!!!!!
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
                int u = edge_v;

                if(graph.exist_edge_[EdgeId(u, v)] == 0){
#ifdef DEBUG_ASSERT
                    assert(graph.exist_vertex_[u] == 0);
#endif
                    continue;
                }

                DeleteVertexInEgoCore(graph, u, v, core[u], core_bucket[u]);
                ego_core_priority_queue.push(EgoCoreOfVertex(core[u].k_, u));
            }
            graph.DeleteVertex(v);
        }
    }
    return ego_core_del_line;
}
long long HashTwo(int x, int y){
    return x * 1000000007LL + y;
}

//note:wrong wrong wrong
EgoCoreDelLine EgoCoreFast(Graph graph){

    //note you should refine core bucket to support more O(1) opreations!

    EgoCoreDelLine ego_core_del_line;
    int n = graph.n_;
    int m = graph.m_;
    int all_graph_id = 0;
    vector<EgoVertex> ego_vertex_vec;
    vector<int> deg;
    vector<int> ego_graph_n;
    unordered_map<long long, int> ori_v_hash;
    vector<int> core_k;
    CoreBucket core_bucket;
    core_bucket.bin_.resize(2*m + 10);
    core_bucket.pos_.resize(2*m + 10);
    core_bucket.vert_.resize(2*m + 10);


    for(int ego = 0; ego < n; ego++){
        ego_graph_n.push_back(graph.edge_[ego].size());

        core_k.push_back(INF);
        for(auto edge_ego:graph.edge_[ego]){
            int u = edge_ego;
            Neighbor neighbor_u_v = graph.NeighborTwoVertex(u, ego);
            ego_vertex_vec.push_back(EgoVertex(u, ego));
            deg.push_back(neighbor_u_v.size());

            core_k[ego] = min(core_k[ego], deg[all_graph_id]);
            LOG3(ego,u,HashTwo(ego, u))
            ori_v_hash[HashTwo(ego, u)] = all_graph_id;
            all_graph_id++;

        }
        if(ego_graph_n[ego] == 0){
            if(ego_core_del_line.size() == 0 || 0 > ego_core_del_line[ego_core_del_line.size()-1].k_){
                CoreDelVertex del_vertex;
                del_vertex.k_ = 0;
                del_vertex.del_vertex_.push_back(ego);
                ego_core_del_line.push_back(del_vertex);
            }else{
                ego_core_del_line[ego_core_del_line.size()-1].del_vertex_.push_back(ego);
            }
            graph.DeleteVertex(ego);
        }
    }
    assert(all_graph_id == 2 * m);
    int md = 0;
    for (auto deg_v:deg) md = max(md, deg_v);
    for (int d = 0; d <= md; d++) core_bucket.bin_[d] = 0;
    for (auto deg_v:deg) core_bucket.bin_[deg_v]++;
    int start = 0;
    for (int d = 0; d <= md; d++) {
        int num = core_bucket.bin_[d];
        core_bucket.bin_[d] = start;
        start += num;
    }
    for (int v = 0; v < ego_vertex_vec.size(); v++) {
        core_bucket.pos_[v] = core_bucket.bin_[deg[v]];
        core_bucket.vert_[core_bucket.pos_[v]] = v;
        core_bucket.bin_[deg[v]]++;
    }

    for (int d = md; d >= 1; d--) {
        core_bucket.bin_[d] = core_bucket.bin_[d - 1];
    }
    core_bucket.bin_[0] = 0;
    core_bucket.start_ = 0;
    // start count  largest ego core'k
    //note: v: labeded of all ego graph vertex//ego1(1,2,3..,n1) ego2(n1+1,,,...n1+n2)....
    LOG("start")
    for(int &st = core_bucket.start_; st < ego_vertex_vec.size() ; st++){

        int v = core_bucket.vert_[st];
        //LOG2(v, deg[v])
        int deg_v = deg[v];
        int ori_v = ego_vertex_vec[v].v_;
        int ego = ego_vertex_vec[v].ego_;
        LOG4(v, ego, ori_v, deg[v] )
        assert(deg[v] >= 0);
        ModifyLinaerQueue(v , deg[v] - 1, deg, core_bucket.bin_, core_bucket.vert_, core_bucket.pos_); //may be wrong if deg_v == 0
        // latency delete v may be deleted to  sure the right of core bucket
        //   assert(ego_graph_n[ego] >= 0);



        if(graph.exist_vertex_[ori_v]) {
            LOG3(ego, ori_v, ego_graph_n[ego] )
            ego_graph_n[ego]--;
            core_k[ego] = max(core_k[ego], deg_v);
            Neighbor neighbor_ego_ori_v = graph.NeighborTwoVertex(ego, ori_v);
            for (auto ori_w:neighbor_ego_ori_v) {
                int w = ori_v_hash[HashTwo(ego, ori_w)];


                if(deg[w] > deg_v)ModifyLinaerQueue(w, deg[w] - 1, deg, core_bucket.bin_, core_bucket.vert_,
                                      core_bucket.pos_);
            }
        }
        if(ego_graph_n[ego] == 0 && graph.exist_vertex_[ego] ){
            LOG(ego)
            //Todo: 1.ego_core_del_line 2.calculate right each related ego core'k after delete v
            if(ego_core_del_line.size() == 0 || core_k[ego] + 1 > ego_core_del_line[ego_core_del_line.size()-1].k_){
                CoreDelVertex del_vertex;
                del_vertex.k_ = core_k[ego] + 1;
                del_vertex.del_vertex_.push_back(ego);
                ego_core_del_line.push_back(del_vertex);
            }else{
                ego_core_del_line[ego_core_del_line.size()-1].del_vertex_.push_back(ego);
            }

          // assert(graph.exist_vertex_[ego] == 1);
            for(auto edge_ego:graph.edge_[ego]) {
                int ori_u = edge_ego;

                if(graph.exist_edge_[EdgeId(ego, ori_u)] == 0){
#ifdef DEBUG_ASSERT
                    assert(graph.exist_vertex_[ori_u] == 0);
#endif
                    continue;
                }

                long long ego_u = HashTwo(ori_u, ego);

                if(deg[ori_v_hash[ego_u]] <= -1) continue;
                ego_graph_n[ori_u]--;
                Neighbor neighbor_oriu_ego = graph.NeighborTwoVertex(ori_u, ego);
                for(auto ori_w: neighbor_oriu_ego){
                    int w = ori_v_hash[HashTwo(ori_u, ori_w)];

                    // LOG4("decdeg",ori_w +s1, core_bucket.start_, core.deg_[core_bucket.vert_[core_bucket.start_]])
                    if(core_bucket.pos_[w] > st)
                        ModifyLinaerQueue(w, deg[w] - 1, deg, core_bucket.bin_, core_bucket.vert_, core_bucket.pos_);
                    core_k[ori_u] = min(core_k[ori_u], deg[w]);
                }
            }
            graph.DeleteVertex(ego);
        }


    }
    for(int u = 0; u < n; u++) LOG2(u, ego_graph_n[u])
    return ego_core_del_line;
}