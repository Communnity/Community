//
// Created by Jiyang Gong on 2019-03-28.
//

#include "graph.h"
long long EdgeId(int u, int v){
    if(u > v) swap(u, v);
    return u * 1000000007LL + v;
}
bool Graph::Empty() {

    for(int u = 0; u < this->n_; u++) if(this->exsit_vertex_[u]) return false;
    return true;
}
void DeleteVertex(Graph &graph, int u){
    graph.exsit_vertex_[u] = 0;
    for(auto edge_u:graph.edge_[u]){
        int v = edge_u.point_;

        if(graph.exsit_vertex_[v] == 0)continue;
        graph.exsit_edge_[EdgeId(u, v)] = 0;
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
    srand(1);
    this->n_ = ran(16);
    this->m_ = ran(this->n_ * (this->n_ - 1) / 2);
    this->Init();
    for(int u = 0; u < this->n_; ++u){
        this->exsit_vertex_[u] = 1;
    }

    for(int i = 0; i < this->m_; i++){
        int u,v;
        do {
            u = ran(this->n_);
            v = ran(this->n_);
            u--;
            v--;
        }while(u == v || this->exsit_edge_[EdgeId(u, v)]);
        this->exsit_edge_[EdgeId(u,v)] = 1;
        this->edge_[u].push_back(Edge(v,0));
        this->edge_[v].push_back(Edge(u,0));
    }
}
void Graph:: InputGraph(){
    read(this->n_); read(this->m_);
    this->Init();
    for(int u = 0; u < this->n_; ++u){
        this->exsit_vertex_[u] = 1;
    }

    for(int i = 0; i < this->m_; ++i){
        int x,y;
        double w1,w2;
        read(x);read(y);//scanf("%f %f",&w1,&w2);
        x--;y--;//[0,N)
        //cout<<x<<" "<<y<<" "<<w1<<" "<<w2<<endl;

        this->edge_[x].push_back(Edge(y,0));
        this->edge_[y].push_back(Edge(x,0));
        assert(this->exsit_edge_[EdgeId(x, y)] == 0);
        this->exsit_edge_[EdgeId(x, y)] = 1;
    }


    cout << "Finish "<<__FUNCTION__<<endl;
}
void Graph:: InputGraphWithAttribute(){
    read(this->n_); read(this->m_);
    this->Init();
    for(int i = 0; i < this->n_; ++i){
        scanf("%d", &this->vertex_[i].attribute_);
    }

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
        this->exsit_vertex_[u] = 1;
        for(auto edge_u:this->edge_[u]){
            int v = edge_u.point_;
            this->exsit_edge_[EdgeId(u, v)] = 1;
        }
    }
    cout << "Finish "<<__FUNCTION__<<endl;
}
void Graph::OutputGraph() {
    for(int u = 0; u < this->n_; u++) if(this->exsit_vertex_[u])
        printf("%d ",u + 1);
    printf("\n");
    for(int u = 0; u < this->n_; u++){
        for(auto edge_u:this->edge_[u]){
            int v = edge_u.point_;
            if(u > v) continue;
            if(this->exsit_edge_[EdgeId(u, v)]) {
                assert(this->exsit_vertex_[v] == 1);
                printf("%d %d\n", u + 1, v + 1);
            }
        }
    }
}
Graph Graph::RenewGraph(){
    vector<int> id(this->n_);
    int vertex_id = 0;
    Graph new_graph;
    new_graph.Init();
    for(int u = 0; u < this->n_; u++) if(this->exsit_vertex_[u]){
        id[u] = vertex_id++;
    } else
        id[u] = -1;
    new_graph.n_ = vertex_id;
    new_graph.m_ = 0;
    for(int u = 0; u < this->n_; u++) if(this->exsit_vertex_[u]){
        for(auto edge_u:this->edge_[u]){
            int v = edge_u.point_;
            if(!this->exsit_vertex_[v]) continue;
            if(!this->exsit_edge_[EdgeId(u, v)]) continue;
            if(new_graph.exsit_edge_[EdgeId(id[u], id[v])]) continue;
            new_graph.edge_[id[u]].push_back(Edge(id[v],0));
            new_graph.edge_[id[v]].push_back(Edge(id[u],0));
            new_graph.exsit_edge_[EdgeId(id[u], id[v])] = 1;
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
    this->exsit_vertex_.clear();
    this->exsit_edge_.clear();
}
Graph InputGraphWithAddColor(){
    Graph graph;
    read(graph.n_); read(graph.m_);

    for(int i = 0; i < graph.n_; ++i){
        graph.vertex_[i].attribute_ = rand()%100 + 1;//[1,100]
    }
    for(int i = 0; i < graph.m_; ++i){
        int x,y;
        double w1,w2;
        read(x);read(y);scanf("%f %f",&w1,&w2);
        x--;y--;//[0,N)
        //cout<<x<<" "<<y<<" "<<w1<<" "<<w2<<endl;

        graph.edge_[x].push_back(Edge(y,0));
        graph.edge_[y].push_back(Edge(x,0));
    }

    return graph;
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
