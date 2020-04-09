//
// Created by Jiyang Gong on 2020-04-07.
//

#include "tree.h"

Tree::Tree(int n) {
  this->root_ = 0; // init
  this->n_ = n;
  son_.resize(n);
}
void Tree::BuildTree(const Graph &graph, vector<bool> &used, int x) {
  used[x] = 1;
  for (auto attribute : graph.vertex_[x].attribute_) {
    this->vertex_[x].attribute_.push_back(attribute);
  }
  for (auto y : graph.edge_[x]){
    if (used[y.point_]) continue;
    this->son_[x].push_back(y);
  }
}
void AttributeTree::ConnectDp(int x) {

//  for (int i = 0; i < this->base_.size(); ++i) {
//    int attribute = 0, number = 0;
//    tie(attribute, number) = this->base_[i];
//    this->dp_[x][number] = 1;
//  }
//  for (auto son : this->son_[x]) {
//    int y = son.point_;
//    ConnectDp(y);
//    for (int status = 0; status < this->dp_[x].size(); ++status) {
//      if (this->dp_[x][status] == 1e9) continue;
//      for (int j  = 0; j < this->vertex_[x].size)
//    }
//  }

}
void AttributeTree::ConnectDpInit(vector<tuple<int, int> > &queries){ // query = {attribute, number}
  int status = 1;
  sort(queries.begin(), queries.end());
  this->base_.resize(queries.size());
  for (int i = 0; i < queries.size(); ++i) {
    int attribute = 0, number = 0;
    tie(attribute, number)  = queries[i];
    this->base_[i + 1] = make_tuple(attribute, status);
    status = status * (number + 1);
  }
  this->dp_ = vector<vector<int> >(this->n_, vector<int>(status, 1e9));
}

