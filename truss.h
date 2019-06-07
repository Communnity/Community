//
// Created by Jiyang Gong on 2019-05-13.
//

#ifndef COMMUNTITY_TRUSS_H
#define COMMUNTITY_TRUSS_H
#include "include.h"
#include "graph.h"
typedef vector<SupEdge> Truss_sup;
Truss_sup FindTruss(Graph graph);
Truss_sup ComputerSup(Graph graph);
Graph BuildTruss(Truss_sup truss_sup, int truss_k);

#endif //COMMUNTITY_TRUSS_H
