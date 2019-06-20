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
Neighbor Graph::NeighborOneVertex(int u){
    Neighbor neighbor_u;
    for(auto edge_u:this->edge_[u]){
        int v = edge_u.point_;
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
        int w = edge_use_u.point_;
        if (this->exist_edge_.find(EdgeId(use_v, w)) != this->exist_edge_.end()) {
            neighbor_u_v.push_back(w);
        }
    }
    return neighbor_u_v;
}
void Graph:: DeleteVertex(int u){
    this->exist_vertex_[u] = 0;
    for(auto edge_u:this->edge_[u]){
        int v = edge_u.point_;

        if(this->exist_vertex_[v] == 0)continue;
        this->exist_edge_[EdgeId(u, v)] = 0;
    }

}

void Graph:: Init(){
    for(int i =0 ; i < this->n_; ++i){
        Vertex vertex;vertex.attribute_ = 0;
        vertex_.push_back(vertex);
    }
    for(int i = 0; i < this->n_; ++i){
        vector<Edge> empty_edge;
        edge_.push_back(empty_edge);
    }
}
void Graph:: RandomInputGraph(){

    this->n_ = 4;
    this->m_ = ran(this->n_ * (this->n_ - 1) / 2);
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
        this->edge_[u].push_back(Edge(v,0));
        this->edge_[v].push_back(Edge(u,0));
    }
}
void Graph:: InputGraph(){
    read(this->n_); read(this->m_);
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
        assert(x != y);
        assert(this->exist_edge_[EdgeId(x, y)] == 0);
        this->edge_[x].push_back(Edge(y,0));
        this->edge_[y].push_back(Edge(x,0));

        this->exist_edge_[EdgeId(x, y)] = 1;
    }


    cout << "Finish "<<__FUNCTION__<<endl;
}
void Graph:: InputGraphWithAttribute(){
    read(this->n_); read(this->m_);
    this->Init();


    for(int i = 0; i < this->m_; ++i){
        int x,y;
        double w1,w2;
        read(x);read(y);//scanf("%f %f",&w1,&w2);
        x--;y--;//[0,N)
        //cout<<x<<" "<<y<<" "<<w1<<" "<<w2<<endl;
        this->edge_[x].push_back(Edge(y,0));
        this->edge_[y].push_back(Edge(x,0));
    }

    for(int u = 0; u < this->n_; ++u){
        this->exist_vertex_[u] = 1;
        for(auto edge_u:this->edge_[u]){
            int v = edge_u.point_;
            this->exist_edge_[EdgeId(u, v)] = 1;
        }
    }
    for(int i = 0; i < this->n_; ++i){
        scanf("%d", &this->vertex_[i].attribute_);
    }
    cout << "Finish "<<__FUNCTION__<<endl;
}
void Graph::OutputGraph() {
    for(int u = 0; u < this->n_; u++) if(this->exist_vertex_[u])
        printf("%d ",u + 1);
    printf("\n");
    for(int u = 0; u < this->n_; u++)if(this->exist_vertex_[u]){
        for(auto edge_u:this->edge_[u]){
            int v = edge_u.point_;
            if(u > v) continue;
            if(this->exist_edge_[EdgeId(u, v)]) {
                assert(this->exist_vertex_[v] == 1);
                printf("%d %d\n", u + 1, v + 1);
            }
        }
    }
}
Graph Graph::RenewGraph(){
    Graph new_graph;
    new_graph.n_ = this->n_;
    new_graph.m_ = 0;
    new_graph.Init();
    for(int u = 0; u < new_graph.n_; u++)
        new_graph.exist_vertex_[u] = this->exist_vertex_[u];
    for(int u = 0; u < this->n_; u++) if(this->exist_vertex_[u]){
            for(auto edge_u:this->edge_[u]){
                int v = edge_u.point_;
                if(!this->exist_vertex_[v]) continue;
                if(!this->exist_edge_[EdgeId(u, v)]) continue;
                if(new_graph.exist_edge_[EdgeId(u, v)]) continue;
                new_graph.edge_[u].push_back(Edge(v,0));
                new_graph.edge_[v].push_back(Edge(u,0));
                new_graph.exist_edge_[EdgeId(u, v)] = 1;
                new_graph.m_++;
            }
        }
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
            int v = edge_u.point_;
            if(!this->exist_vertex_[v]) continue;
            if(!this->exist_edge_[EdgeId(u, v)]) continue;
            if(new_graph.exist_edge_[EdgeId(id[u], id[v])]) continue;
            new_graph.edge_[id[u]].push_back(Edge(id[v],0));
            new_graph.edge_[id[v]].push_back(Edge(id[u],0));
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
            int v = edge_u.point_;
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
    for(int i = 0; i < graph.n_; ++i){
        fprintf(fp, "%d\n",graph.vertex_[i]);//[1,100]
    }
    for(int v = 0; v < graph.m_; ++v){
        for(int i = 0; i < graph.edge_[v].size(); i++){
            int u = graph.edge_[v][i].point_;
            if(v < u) fprintf(fp, "%d %d\n",v,u);
        }
    }
}

Graph::Graph(void) {
}
Graph::Graph(int n, int m){
    this->n_ = n;
    this->m_ = m;
    for(int i =0 ; i < this->n_; ++i){
        Vertex vertex;vertex.attribute_ = 0;
        vertex_.push_back(vertex);
    }
    for(int i = 0; i < this->m_; ++i){
        vector<Edge> empty_edge;
        edge_.push_back(empty_edge);
    }
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
            Edge &edge_v = graph.edge_[v][i];

            if (need[edge_v.point_] && need[v]) {
                sub_graph.edge_[v].push_back(edge_v);
            }
        }
    }
    for(int v = 0; v < n; ++v)  sub_graph.m_ += sub_graph.edge_[v].size();

    //return graph
    return sub_graph;
}
