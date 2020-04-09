//
// Created by Jiyang Gong on 2020-04-08.
//

#ifndef COMMUNITY_GRAPH_ALGORITHM_H
#define COMMUNITY_GRAPH_ALGORITHM_H
#include "graph.h"
class Query {
 public:
  int query_maxsize_;
  int find_core_k_;
  vector<int> search_core_, find_core_;
  vector<int> choose_;
  vector<tuple<int, int>> queries;
  Query(int n);
  void Search(const Graph &graph, int x);
  bool Check(const Graph &graph);
  bool FindMatch(BiGraph &bigraph);
  void Output();
};

#endif //COMMUNITY_GRAPH_ALGORITHM_H
