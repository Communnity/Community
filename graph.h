//
// Created by Jiyang Gong on 2019-03-28.
//

#ifndef Communtity_GRAPH_H
#define Communtity_GRAPH_H

#include "include.h"
const int c_Max_Vertex_Num =  5000010;
const int c_Max_Edge_Num = 50010010;
typedef  vector<int> Neighbor;
typedef  vector<int> Core;

class Edge{
    public:
        int point_;
        int weight_;
        Edge(int point,int weight){point_ = point; weight_ = weight;};
};
class Vertex{
    public:
        int attribute_;
};
class XYEdge{
    public:
        int v_;
        int u_;
};
class SupEdge:public XYEdge{
    public:
        int sup_;
        SupEdge(int v,int u,int sup){u_ = u, v_  = v, sup_ = sup; };
        SupEdge(){};

};
class Graph{
    public:
        int n_;
        int m_;
        vector<vector<Edge> > edge_;
        vector<Vertex> vertex_;
        unordered_map<long long, int> exist_edge_;
        unordered_map<int, int> exist_vertex_;
        void Init();
        Graph(int n, int m);
        Graph();
        bool Empty();
        void InputGraph();
        void InputGraphWithAttribute();
        void OutputGraph();
        void OutputOriginalGraph();
        Graph RenewGraphwithReid();
        Graph RenewGraph();
        int CalEdgeNum();
        void RandomInputGraph();
        void ClearGraph();
        Neighbor NeighborOneVertex(int u);
        Neighbor NeighborTwoVertex(int u, int v);
        bool empty();
        void DeleteVertex(int u);
};

class EgoGraph:public Graph{
    public:
        int ego;
};

Graph ExtractSubgraph(Graph g,vector<int> need);

Graph InputGraphWithAddColor();
void OutputGraph(Graph graph);
long long EdgeId(int u, int v);
#endif //Communtity_GRAPH_H

