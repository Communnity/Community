//
// Created by Jiyang Gong on 2020-04-08.
//

#include "graph_algorithm.h"
bool Query::FindMatch(BiGraph &bigraph) {
  if (bigraph.MaxMatch() == bigraph.leftn_) return true;
  return false;
}
bool Query::Check(const Graph &graph) {

  int n1 = this->search_core_.size();
  if (n1 < this->query_sum_) return false;
  int n2 = 0;
  int search_core_k = 1e9;
  for (auto x : this->search_core_) {
    int cnt = 0;
    for (auto edge : graph.edge_[x]) {
      int y = edge;
      if (this->choose_[y]) ++cnt;
    }
    search_core_k = min(search_core_k, cnt);
  }
  if (search_core_k < find_core_k_) return false;
  if (search_core_k == find_core_k_ && this->search_core_.size() >= find_core_.size()) return false;



  int leftn = 0;
  for (int i = 0; i < queries.size(); ++i)
    leftn += get<1>(queries[i]);
  BiGraph bigraph(leftn, this->search_core_.size());

  vector<vector<int>> assign(graph.max_attribute_ + 1, vector<int>());//[1,max_attribute_]

  for (int j = 0; j < this->search_core_.size(); j++) {
    for(auto attribute : graph.vertex_[this->search_core_[j]].attribute_)
      assign[attribute].push_back(j);
  }

  int n = 0;
  for (int i = 0; i < queries.size(); ++i) {
    int attribute, number;
    tie(attribute, number) = queries[i];
    for (int i = 1; i <= number; ++i) {
      for (auto x : assign[attribute]) {
        bigraph.edge_[n].push_back(x);
      }
      ++n;
    }
  }

  if (this->FindMatch(bigraph)) {
    find_core_k_ = search_core_k;

    this->find_core_.clear();
    for (auto x :  this->search_core_) this->find_core_.push_back(this->id_[x]);
    return true;
  } else {
    return false;
  }

  return true;

}
Query::Query(int n, string input_str) {
  this->choose_.resize(n);
  find_core_k_ = -1;
  freopen(("../cmake-build-debug/" + input_str).c_str(), "r", stdin);
  int nq;
  query_sum_ = 0;
  read(nq);read(query_maxsize_);
  for (int i = 1; i <= nq; ++i) {
    int attribute, number;
    read(attribute); read(number);
    this->queries.push_back(make_tuple(attribute, number));
    query_sum_ += number;
  }
}

void Query::Search(const Graph &graph, int x) {

  if (x == graph.n_) return;
  if (this->search_core_.size() > this->query_maxsize_) {
    return;
  }
  if(this->Check(graph)) {
    return;
  }


  int cnt = 0, connected = 0;
  for (int i = 0; i < graph.edge_[x].size(); ++i) {
    if (this->choose_[graph.edge_[x][i]]) cnt++;
    else if (graph.edge_[x][i] > x) cnt++;
    if (this->choose_[graph.edge_[x][i]]) connected = 1;
  }
  if (connected && !(cnt < this->find_core_k_ || (cnt  == this->find_core_k_ && search_core_.size() >= this->find_core_.size()))) {
    this->choose_[x] = 1;
    this->search_core_.push_back(x);
    Search(graph, x + 1);
    this->search_core_.pop_back();
    this->choose_[x] = 0;
  }


  Search(graph, x + 1);
}

void Query::Output() {
  cout << "Find_Core_K="<<this->find_core_k_ << endl;
  for (auto x : this->find_core_) cout << x + 1<< " "; cout << endl;
}

Graph  Query:: ReidGraph(const Graph &graph, vector<int> & id , int st) {
  queue<int> q;
  q.push(st);
  vector<int> vis(graph.n_, 0);
  int id_now = 0;
  id[st] = id_now;
  vis[st] = 1;
  while (!q.empty()) {
    int x = q.front();
    q.pop();
    for (auto y : graph.edge_[x]) {
      if (vis[y]) continue;
      vis[y] = 1;
      q.push(y);
      id[y] = ++id_now;
    }
  }
  assert(id_now == graph.n_ - 1);
  Graph reid_graph(graph.n_);
  reid_graph.max_attribute_ = graph.max_attribute_;
  for (int i = 0; i < graph.n_; ++i) {
    for (auto y : graph.vertex_[i].attribute_)
      reid_graph.vertex_[id[i]].attribute_.push_back(y);
    for (auto y : graph.edge_[i]) {
      reid_graph.edge_[id[i]].push_back(id[y]);
    }
  }
  return reid_graph;
}

void Query::Start(const Graph &graph) {
//    query.Search(graph, 0);
  for (int i = 0; i < graph.n_; ++i) {
    vector<int> id(graph.n_);
    Graph reid_graph = this->ReidGraph(graph, id, i);
    this->id_.resize(graph.n_);
    for (auto x : id) {
      this->id_[id[x]] = x;
    }
    this->search_core_.push_back(0);
    this->choose_[0] = 1;
    this->Search(reid_graph, 1);
    this->search_core_.pop_back();
    this->choose_[0] = 0;
  }
}