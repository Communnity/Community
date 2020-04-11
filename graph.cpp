//
// Created by Jiyang Gong on 2019-03-28.
//

#include "graph.h"
long long EdgeId(int u, int v){
    if(u > v) swap(u, v);
    return u * 1000000007LL + v;
}
bool Graph::Empty() {

    for(int u = 0; u < this->n_; u++) if(this->exist_vertex_[u]) return false;
    return true;
}
//bool EdgeCmp(Edge e1, Edge e2){
//    return e1.point_ < e2.point_;
//}
Neighbor Graph::NeighborOneVertex(int u){
    Neighbor neighbor_u;
    for(auto edge_u:this->edge_[u]){
        int v = edge_u;
        if(!this->exist_vertex_[v])continue;
        assert(this->exist_edge_[EdgeId(u, v)] == 1);
        neighbor_u.push_back(v);
    }
    sort(neighbor_u.begin(), neighbor_u.end());
    return neighbor_u;
}
Neighbor Graph::NeighborTwoVertex(int u, int v){
//    Neighbor neighbor_u, neighbor_v,neighbor_u_v;
//    neighbor_u = this->NeighborOneVertex(u);
//    neighbor_v = this->NeighborOneVertex(v);
//
//    int u_i = 0, v_i = 0;
//    while(u_i < neighbor_u.size() && v_i < neighbor_v.size()){
//        if(neighbor_u[u_i] < neighbor_v[v_i]) u_i++;
//        else if(neighbor_u[u_i]>neighbor_v[v_i]) v_i++;
//        else if(neighbor_u[u_i] == neighbor_v[v_i]){
//            neighbor_u_v.push_back(neighbor_u[u_i]);
//            u_i++;
//            v_i++;
//        }
//        else assert(0);
//    }
    Neighbor neighbor_u_v;
    int use_u = u, use_v = v;
    int deg_u = 0;
    if (this->edge_[v].size() < this->edge_[u].size()) swap(use_u, use_v);
    for (auto edge_use_u:this->edge_[use_u]) {
        int w = edge_use_u;
        if (this->exist_edge_.find(EdgeId(use_v, w)) != this->exist_edge_.end()) {
            neighbor_u_v.push_back(w);
        }
    }
    return neighbor_u_v;
}
void Graph:: DeleteVertex(int u){
    this->exist_vertex_[u] = 0;
    for(auto edge_u:this->edge_[u]){
        int v = edge_u;

        if(this->exist_vertex_[v] == 0)continue;
        this->exist_edge_[EdgeId(u, v)] = 0;
    }

}

void Graph:: Init(){

//    for(int i =0 ; i < this->n_; ++i){
//        Vertex vertex;
//        vertex_.push_back(vertex);
//    }
  vertex_ = vector<Vertex> (this->n_);
//    for(int i = 0; i < this->m_; ++i){
//        vector<int> empty_edge;
//        edge_.push_back(empty_edge);
//    }
  edge_ = vector<vector<int> >(this->n_);
}
void Graph:: RandomInputGraph(){

    this->n_ = 10;
    this->m_ = ran(this->n_ * (this->n_ - 1) / 2);
   // this->m_ = this->n_ * (this->n_ - 1) / 2 - 2 * this->n_;

    this->Init();
    for(int u = 0; u < this->n_; ++u){
        this->exist_vertex_[u] = 1;
    }

    for(int i = 0; i < this->m_; i++){
        int u,v;
        do {
            u = ran(this->n_);
            v = ran(this->n_);
            assert(u > 0 && v > 0);
            u--;
            v--;
        }while(u == v || this->exist_edge_[EdgeId(u, v)]);
        this->exist_edge_[EdgeId(u,v)] = 1;
        this->edge_[u].push_back(v);
        this->edge_[v].push_back(u);
    }
}



void Graph:: InputGraph(){
    //freopen("/Users/gjy/Documents/社交网络与图论论文/dataset/gowalla/gowalla.inf","r",stdin);
    //freopen("/Users/gjy/Documents/社交网络与图论论文/dataset/foursquare_extend/foursquare_extend.inf","r",stdin);
     // freopen("/Users/gjy/Documents/社交网络与图论论文/dataset/youtube/com-youtube.ungraph-reid.txt", "r", stdin);
     //freopen("/Users/gjy/Documents/社交网络与图论论文/dataset/facebook/facebook-combined-reid.txt", "r", stdin);
    freopen("/Users/gjy/Documents/社交网络与图论论文/dataset/small/dblp-graph.in", "r", stdin);
    //freopen("/Users/gjy/Documents/社交网络与图论论文/dataset/small/easyclique.in", "r", stdin);
    // freopen("/Users/gjy/Documents/社交网络与图论论文/dataset/com-LiveJournal/com-LiveJournal-ungraph.txt", "r", stdin);

    read(this->n_); read(this->m_);
    cerr<<"n="<<this->n_<<" "<<"m="<<this->m_<<endl;
    this->Init();
    for(int u = 0; u < this->n_; ++u){
        this->exist_vertex_[u] = 1;
    }

    for(int i = 0; i < this->m_; ++i){
        int x,y;
        read(x);read(y);

        assert(x > 0 && x <= this->n_);
        assert(y > 0 && y <= this->n_);
        x--;y--;//[0,N)
        //cout<<x<<" "<<y<<" "<<w1<<" "<<w2<<endl;
        if(x==y)cerr<<x<<" "<<y;
        assert(x != y);

        assert(this->exist_edge_[EdgeId(x, y)] == 0);
        this->edge_[x].push_back(y);
        this->edge_[y].push_back(x);

        this->exist_edge_[EdgeId(x, y)] = 1;
    }


    cerr << "Finish "<<__FUNCTION__<<endl;
}
void Graph:: InputGraphWithAttribute(string input_str){

    freopen(("../cmake-build-debug/" + input_str).c_str(), "r", stdin);
    read(this->n_); read(this->m_);
    this->Init();


    for(int i = 0; i < this->m_; ++i) {
        int x, y;
        read(x);read(y);
        x--;y--;//[0,N)
        //cout<<x<<" "<<y<<" "<<w1<<" "<<w2<<endl;
        this->edge_[x].push_back(y);
        this->edge_[y].push_back(x);
    }

    for(int u = 0; u < this->n_; ++u){
        this->exist_vertex_[u] = 1;
        for(auto edge_u:this->edge_[u]){
            int v = edge_u;
            this->exist_edge_[EdgeId(u, v)] = 1;
        }
    }

    read(this->max_attribute_);
    for(int i = 0; i < this->n_; ++i){
        int k; read(k);
        for (int j = 0; j < k; ++j) {
            int attribute = -1;
            read(attribute);
            this->vertex_[i].attribute_.push_back(attribute);
        }

    }
    cerr << "Finish "<<__FUNCTION__<<endl;
}
void Graph::OutputGraph() {
    for(int u = 0; u < this->n_; u++) if(this->exist_vertex_[u])
        printf("%d ",u + 1);
    printf("\n");
    for(int u = 0; u < this->n_; u++)if(this->exist_vertex_[u]){
        for(auto edge_u:this->edge_[u]){
            int v = edge_u;
            if(u > v) continue;
            if(this->exist_edge_[EdgeId(u, v)]) {
                assert(this->exist_vertex_[v] == 1);
                printf("%d %d\n", u + 1, v + 1);
            }
        }
    }
}
Graph Graph::RenewGraph(){
    Graph new_graph(this->n_);

    for(int u = 0; u < new_graph.n_; u++)
        new_graph.exist_vertex_[u] = 1;
    for(int u = 0; u < this->n_; u++) if(this->exist_vertex_[u]){
            for(auto v:this->edge_[u]){
                if(!this->exist_vertex_[v]) continue;
                if(!this->exist_edge_[EdgeId(u, v)]) continue;
                if(new_graph.exist_edge_[EdgeId(u, v)]) continue;
                new_graph.edge_[u].push_back(v);
                new_graph.edge_[v].push_back(u);
                new_graph.exist_edge_[EdgeId(u, v)] = 1;
                new_graph.m_++;
            }
        }
    for(int u = 0; u < new_graph.n_; u++) {
      for (auto x : this->vertex_[u].attribute_) {
        new_graph.vertex_[u].attribute_.push_back(x);
      }
    }
    cerr << new_graph.m_ << " "<<this->m_ << endl;
    return new_graph;
}
Graph Graph::RenewGraphwithReid(){
    vector<int> id(this->n_);
    int vertex_id = 0;
    Graph new_graph;

    for(int u = 0; u < this->n_; u++) if(this->exist_vertex_[u]){
        id[u] = vertex_id++;
    } else
        id[u] = -1;
    new_graph.n_ = vertex_id;


    new_graph.m_ = 0;
    new_graph.Init();

    for(int u = 0; u < new_graph.n_; u++)
        new_graph.exist_vertex_[u] = 1;
    for(int u = 0; u < this->n_; u++) if(this->exist_vertex_[u]){
        for(auto edge_u:this->edge_[u]){
            int v = edge_u;
            if(!this->exist_vertex_[v]) continue;
            if(!this->exist_edge_[EdgeId(u, v)]) continue;
            if(new_graph.exist_edge_[EdgeId(id[u], id[v])]) continue;
            new_graph.edge_[id[u]].push_back(id[v]);
            new_graph.edge_[id[v]].push_back(id[u]);
            new_graph.exist_edge_[EdgeId(id[u], id[v])] = 1;
            new_graph.m_++;
        }
    }
    return new_graph;
}
void Graph::OutputOriginalGraph()  {
    for(int u = 0; u < this->n_; u++)
            printf("%d ",u + 1);
    printf("\n");
    for(int u = 0; u < this->n_; u++){
        for(auto edge_u:this->edge_[u]){
            int v = edge_u;
            if(u > v) continue;
            printf("%d %d\n",u + 1, v + 1);
        }
    }
}
void Graph::ClearGraph() {
    this->n_ = this->m_ = 0;
    this->vertex_.clear();
    this->edge_.clear();
    this->exist_vertex_.clear();
    this->exist_edge_.clear();
}

void OutputGraph(Graph graph, string file_str){
    char *file_s= StrToChar(file_str);
    FILE *fp = fopen(file_s, "w");

    fprintf(fp, "%d %d\n",graph.n_,graph.m_);

    for(int v = 0; v < graph.m_; ++v){
        for(int i = 0; i < graph.edge_[v].size(); i++){
            int u = graph.edge_[v][i];
            if(v < u) fprintf(fp, "%d %d\n",v,u);
        }
    }
}

Graph::Graph(void) {

}
Graph::Graph(int n){
    this->n_ = n;
    this->m_ = 0;
//    for(int i =0 ; i < this->n_; ++i){
//        Vertex vertex;
//        vertex_.push_back(vertex);
//    }
    vertex_ = vector<Vertex> (n);
//    for(int i = 0; i < this->m_; ++i){
//        vector<int> empty_edge;
//        edge_.push_back(empty_edge);
//    }
    edge_ = vector<vector<int> >(n);
}
int Graph::CalEdgeNum(){
    int m = 0;
    for(int i = 0; i < this->n_; i++){
        m += this->edge_[i].size();
    }
    return m;
}
Graph ExtractSubgraph(Graph graph, vector<int> need){
    int &n = graph.n_;
    //Get vertex message
    Graph sub_graph;
    sub_graph.n_ = n;
    for(int i = 0;i < n; i++){
        sub_graph.vertex_[i]  = graph.vertex_[i];
    }

    //Get edge message;

    for(int v = 0; v < n; v++){


        for(int i = 0; i < graph.edge_[v].size(); i++) {
            int &edge_v = graph.edge_[v][i];

            if (need[edge_v] && need[v]) {
                sub_graph.edge_[v].push_back(edge_v);
            }
        }
    }
    for(int v = 0; v < n; ++v)  sub_graph.m_ += sub_graph.edge_[v].size();

    //return graph
    return sub_graph;
}


BiGraph :: BiGraph(int n1, int n2) {
    edge_  = vector<vector<int> > (n1);
    leftn_ = n1;
    rightn_ = n2;
}
bool BiGraph::SearchP()
{
    queue<int> Q;
    dis_ = INF;
    dx_ = vector<int>(leftn_, -1);
    dy_ = vector<int>(rightn_, -1);
    // 使用BFS遍历对图的点进行分层，从X中找出一个未匹配点v
    // (所有v)组成第一层，接下来的层都是这样形成——每次查找
    // 匹配点(增广路性质)，直到在Y中找到未匹配点才停止查找，
    // 对X其他未匹配点同样进行查找增广路径(BFS只分层不标记
    // 是否匹配点)
    // 找出X中的所有未匹配点组成BFS的第一层
    for (int i = 0; i < mx_.size(); ++i) {
        if (mx_[i] == -1) {
            Q.push(i);
            dx_[i] = 0;
        }
    }
    while (!Q.empty()) {
        int u = Q.front();
        Q.pop();
        // 该路径长度大于dis，等待下一次BFS扩充
        // dis是到Y集合的长度，所以dis一定是一个奇数
        if (dx_[u] > dis_) break;
        for (auto v : this->edge_[u]) {
            // (u,v)之间有边且v还没有分层
            // 分配v的层次
            if (dy_[v] == -1)
            {
                dy_[v] = dx_[u] + 1;
                // v是未匹配\\\\\\\\\点，停止延伸（查找）
                // 得到本次BFS的最大遍历层次
                if (my_[v] == -1) dis_ = dy_[v];
                    // v是匹配点，继续延伸
                else
                {
                    dx_[my_[v]] = dy_[v] + 1;
                    Q.push(my_[v]);
                }
            }
        }
    }
    // 若dis为INF说明Y中没有未匹配点，也就是没有增广路径了
    return dis_ != INF;
}

// 用DFS遍历查找BFS形成的增广路，如果可以找到增广路就停止遍历并返回true
bool BiGraph::DFS(int u) {
    for (auto v: this->edge_[u]) {
        if (!vst_[v] && dy_[v] == dx_[u] + 1) {
            vst_[v] = 1;
            // 层次（也就是增广路径的长度）大于本次查找的dis
            // 是searchP中被break的情况，也就是还不确定是否是增广路
            // 只有等再次调用searchP再判断
            if (my_[v] != -1 && dy_[v] == dis_) continue;
            // 是增广路径，更新匹配集
            if (my_[v] == -1 || DFS(my_[v])) {
                my_[v] = u;
                mx_[u] = v;
                return true;
            }
        }
    }
    return false;
}

// 查找二分图的最大匹配
int BiGraph::MaxMatch() {
    int res = 0;
    // 每个结点的匹配顶点置空
    mx_ = vector<int>(leftn_, -1);
    my_ = vector<int>(rightn_, -1);
    // 如果BFS可以找到增广路径
    while (SearchP()) {
        vst_ = vector<bool>(rightn_, 0);
        for (int i = 0; i < leftn_; ++i) {
            // 用DFS查找增广路径，增广路径一定从未匹配点开始
            // 如果查找到一个增广路径，匹配数加一
            if (mx_[i] == -1 && DFS(i))
                ++res;
        }
    }
    return res;
}