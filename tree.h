//
// Created by Jiyang Gong on 2020-04-07.
//

#ifndef COMMUNITY_TREE_H
#define COMMUNITY_TREE_H
#include "include.h"
#include "graph.h"
class Tree {
 public:
  int root_, n_, max_attribute_;
  vector<Vertex> vertex_;
  vector<vector<int>> son_;
  vector<tuple<int, int> > base_;
  Tree(int n);
  void BuildTree(const Graph &graph, vector<bool> &used, int x);
};

class AttributeTree : public Tree {
 public:
  int max_attr_;
  vector<vector<int> > attribute_node_;
  void ConnectDp(int x);
  void ConnectDpInit(vector<tuple<int, int> > &queries);
  vector<vector<int> > dp_;
};

#endif //COMMUNITY_TREE_H
