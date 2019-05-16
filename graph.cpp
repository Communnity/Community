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
    for(int i = 0; i < this->m_; ++i){
        vector<Edge> empty_edge;
        edge_.push_back(empty_edge);
    }
}

void Graph:: InputGraph(){
    read(this->n_); read(this->m_);
    this->Init();
    for(int i = 0; i < this->n_; ++i){
        scanf("%d", &this->vertex_[i]);
    }

    for(int i = 0; i < this->m_; ++i){
        int x,y;
        double w1,w2;
        read(x);read(y);scanf("%f %f",&w1,&w2);
        x--;y--;//[0,N)
        //cout<<x<<" "<<y<<" "<<w1<<" "<<w2<<endl;
        Edge edge_x,edge_y;
        edge_x.point_=y;
        edge_y.point_=x;
        this->edge_[x].push_back(edge_x);
        this->edge_[y].push_back(edge_y);
    }

    for(int u = 0; u < this->n_; ++u){
        this->exsit_vertex_[u] = 1;
        for(auto edge_u:this->edge_[u]){
            int v = edge_u.point_;
            this->exsit_edge_[EdgeId(u, v)] = 1;
        }
    }
}

void Graph::OutputGraph() {
    for(int u = 0; u < this->n_; u++) if(this->exsit_vertex_[u])
        printf("%d ",u + 1);
    printf("\n");
    for(int u = 0; u < this->n_; u++){
        for(auto edge_u:this->edge_[u]){
            int v = edge_u.point_;
            if(this->exsit_edge_[EdgeId(u, v)])
                printf("%d %d\n",u + 1, v + 1);
        }
    }
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
        Edge edge_x,edge_y;
        edge_x.point_=y;
        edge_y.point_=x;
        graph.edge_[x].push_back(edge_x);
        graph.edge_[y].push_back(edge_y);
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
