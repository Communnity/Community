//
// Created by Jiyang Gong on 2019-03-28.
//

#ifndef Communtity_GRAPH_H
#define Communtity_GRAPH_H

#include "include.h"
const int c_Max_Vertex_Num =  5000010;
const int c_Max_Edge_Num = 50010010;
class Edge{
    public:
        int point_;
        int weight_;
};
class Vertex{
    public:
        int attribute_;

};
class Graph{
    public:
        int n_;
        int m_;
        vector<vector<Edge> > edge_;
        vector<Vertex> vertex_;
        Graph();
        void InputGraph();
};

class EgoGraph:public Graph{
    public:
        int ego;
};

Graph ExtractSubgraph(Graph g,vector<int> need);
Graph InputGraph();
Graph InputGraphWithAddColor();
void OutputGraph(Graph graph);
#endif //Communtity_GRAPH_H

