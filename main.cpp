
#include "include.h"
#include "core.h"
#include "truss.h"
#include "ks_core.h"
#include "graph.h"
#include "file_manage.h"
#include "ego_community.h"
#include "clique.h"
#include "tree.h"
#include "graph_algorithm.h"
void TestDataset(Graph &graph){

    clock_t start,finish; //定义开始，结束变量
    double duration;
    // Get core'K
    start = clock();//初始化
    CoreDelLine core_del_line = FindCore(graph);
    cout << "Core'K="<<core_del_line[core_del_line.size()-1].k_<<endl;
    sort(core_del_line[core_del_line.size()-1].del_vertex_.begin(), core_del_line[core_del_line.size()-1].del_vertex_.end());
    for(auto v: core_del_line[core_del_line.size()-1].del_vertex_)cout<<v<<" ";cout<<endl;
    finish = clock();//初始化结束时间
    duration = (double)(finish - start) / CLOCKS_PER_SEC;//转换浮点型
    printf( "TestCore %lf seconds\n", duration );

//Get truss'K
    start = clock();//初始化
    Truss_sup truss = FindTruss(graph);
    int max_truss = 0;
    for (int i = 0; i < graph.m_; i++) max_truss = max(max_truss, truss[i].sup_);
    cout << "Truss_sup'K="<<max_truss << endl;
    finish = clock();//初始化结束时间
    duration = (double)(finish - start) / CLOCKS_PER_SEC;//转换浮点型
    printf( "TestTruss %lf seconds\n", duration );

//Get the number of triangle
    start = clock();//初始化
    Truss_sup sup_edge = ComputerSup(graph);
    int sum_truss = 0;
    for(int i = 0; i < graph.m_; i++) sum_truss += sup_edge[i].sup_;
    cout<<"the number of triangles="<< sum_truss/3 <<endl;
    finish = clock();//初始化结束时间
    duration = (double)(finish - start) / CLOCKS_PER_SEC;//转换浮点型
    printf( "Testtriangle %lf seconds\n", duration );


//Get ego_core'K
    start = clock();//初始化
    EgoCoreDelLine  ego_core_del_line = EgoCore(graph);
    cout << "Ego_Core'K="<<ego_core_del_line[ego_core_del_line.size()-1].k_<<endl;
    sort(ego_core_del_line[ego_core_del_line.size()-1].del_vertex_.begin(),ego_core_del_line[ego_core_del_line.size()-1].del_vertex_.end());
    for(auto v: ego_core_del_line[ego_core_del_line.size()-1].del_vertex_)cout<<v<<" ";cout<<endl;
    finish = clock();//初始化结束时间
    duration = (double)(finish - start) / CLOCKS_PER_SEC;//转换浮点型
    printf( "TestEgo_Core %lf seconds\n", duration );

    //Get ego_core_fast'K
//    start = clock();//初始化
//    EgoCoreDelLine ego_core_del_line_fast = EgoCoreFast(graph);
//    cout << "Ego_Core_Fast'K="<<ego_core_del_line_fast[ego_core_del_line_fast.size()-1].k_<<endl;
//    finish = clock();//初始化结束时间
//    duration = (double)(finish - start) / CLOCKS_PER_SEC;//转换浮点型
//    printf( "TestEgo_Core %lf seconds\n", duration );
//Get ego_core_baseline'K
//    EgoCoreDelLine ego_core_del_line_bl = EgoCoreBaseline(graph);
//    cout << "Ego_Core_baseline'K="<<ego_core_del_line_bl[ego_core_del_line_bl.size()-1].k_<<endl;
//    sort(ego_core_del_line_bl[ego_core_del_line_bl.size()-1].del_vertex_.begin(),ego_core_del_line_bl[ego_core_del_line_bl.size()-1].del_vertex_.end());
//    for(auto v: ego_core_del_line_bl[ego_core_del_line_bl.size()-1].del_vertex_)cout<<v<<" ";cout<<endl;


}
void Print(EgoCoreDelLine ego_core_del_line, string str){
    for(auto del_line:ego_core_del_line) {
        LOG2(str, del_line.k_);
        for (auto v:del_line.del_vertex_) {
            LOG(v + 1)
        }
    }
}
int  Compapre(EgoCoreDelLine &ego_core_del_line,EgoCoreDelLine &ego_core_del_line_bl){
    if(ego_core_del_line.size() != ego_core_del_line_bl.size()) return 0;
    for(int i = 0; i < ego_core_del_line.size();i++){
        if(ego_core_del_line[i].k_ != ego_core_del_line_bl[i].k_) return 0;
        if(ego_core_del_line[i].del_vertex_.size() != ego_core_del_line_bl[i].del_vertex_.size()) return 0;
        for(int j = 0; j < ego_core_del_line[i].del_vertex_.size(); j++)
            if(ego_core_del_line[i].del_vertex_[j] != ego_core_del_line_bl[i].del_vertex_[j]) return 0;
    }
    return 1;
}
void TestEgo(){
    Graph graph;
    EgoCoreDelLine ego_core_del_line_bl,ego_core_del_line;
    do {
        graph.ClearGraph();
        graph.InputGraph();

        ego_core_del_line_bl = EgoCoreBaseline(graph);
        ego_core_del_line = EgoCore(graph);

        Print(ego_core_del_line,"ego_core_del_line");
        for(int i = 0; i < ego_core_del_line.size();i++) sort(ego_core_del_line[i].del_vertex_.begin(), ego_core_del_line[i].del_vertex_.end());
        for(int i = 0; i < ego_core_del_line_bl.size();i++) sort(ego_core_del_line_bl[i].del_vertex_.begin(), ego_core_del_line_bl[i].del_vertex_.end());
    }while(Compapre(ego_core_del_line,ego_core_del_line_bl) == 1);
    graph.OutputOriginalGraph();
    Print(ego_core_del_line,"ego_core_del_line");
    Print(ego_core_del_line_bl,"ego_core_del_line_bl");

}
void TestClique(){
    Graph graph;
    graph.InputGraph();
    for(int u = 0; u < graph.n_; u++){
        sort(graph.edge_[u].begin(), graph.edge_[u].end());
    }
    vector<int> empty_set, P;
    int mc_size = 0;
    for(int i = 0; i < graph.n_; i++) P.push_back(i);

    BasicMCE(graph, P, empty_set, empty_set, mc_size);
    graph.OutputOriginalGraph();
    cout<<mc_size<<endl;

}
void CompareTrussandEgocore(){
    do{
        Graph graph;
        graph.RandomInputGraph();
        Truss_sup truss_sup = FindTruss(graph);
        int max_truss_k = 0;
        for (int i = 0; i < graph.m_; i++) max_truss_k = max(max_truss_k, truss_sup[i].sup_);
        EgoCoreDelLine ego_core_del_line = EgoCoreBaseline(graph);
        int max_ego_core_k = ego_core_del_line[ego_core_del_line.size()-1].k_;
        cout<<"Truss_sup'k="<<max_truss_k<<" "<<"ego_core'k="<<max_ego_core_k<<endl;

        Graph truss = BuildTruss(truss_sup, max_truss_k);
        Graph ego_core = BuildCorewithIndex(graph, ego_core_del_line, max_ego_core_k);
        if(ego_core.n_ != ego_core_del_line[ego_core_del_line.size()-1].del_vertex_.size())
            cout<<ego_core.n_<< " "<< ego_core_del_line[ego_core_del_line.size()-1].del_vertex_.size()<<endl;
        assert(ego_core.n_ == ego_core_del_line[ego_core_del_line.size()-1].del_vertex_.size());
        cout<<"Truss_n="<<truss.n_<<"ege_core_n="<<ego_core.n_<<endl;
        cout<<"Truss_m="<<truss.m_<<"ege_core_m="<<ego_core.m_<<endl;
        if(max_truss_k + 2 < max_ego_core_k){
            graph.OutputGraph();
            break;
        }
    }while(1);
}

int main(int argc, char *argv[]) {

    srand(time(NULL));


    Graph graph;
    graph.InputGraphWithAttribute((string)argv[1]);

    clock_t start,finish; //定义开始，结束变量
    start = clock();//初始化
    Query query(graph.n_, (string)argv[2]);
    query.Start(graph);
    finish = clock();//初始化结束时间
    double duration = (double)(finish - start) / CLOCKS_PER_SEC;//转换浮点型
    printf( "TestDataset %lf seconds\n", duration );

    query.Output();
    return 0;
}