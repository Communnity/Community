//
// Created by Jiyang Gong on 2020-04-08.
//

#ifndef COMMUNITY_GRAPH_ALGORITHM_H
#define COMMUNITY_GRAPH_ALGORITHM_H
#include "graph.h"
#define Test
class Query {
 public:
  int query_maxsize_, query_sum_, cost_;
  int find_core_k_;
  vector<int> search_core_, find_core_;
  vector<int> choose_;
  vector<int> id_;
  vector<tuple<int, int>> queries;
  Query(int n, string input_str);
  void Output();
  void Start(const Graph &graph);
 private:
  Graph ReidGraph(const Graph &graph, vector<int> & id , int st);
  bool Check(const Graph &graph);
  bool FindMatch(BiGraph &bigraph);
  void Search(const Graph &graph, int x);
  int ExpectedKCore(const Graph &graph, int limit);
};

#endif //COMMUNITY_GRAPH_ALGORITHM_H
