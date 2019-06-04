
#include "include.h"
#include "cores.h"
#include "truss.h"
#include "ks_core.h"
#include "graph.h"
#include "file_manage.h"


int main() {

    //freopen("/Users/gjy/Documents/社交网络与图论论文/dataset/gowalla/gowalla.inf","r",stdin);
    // freopen("/Users/gjy/Documents/社交网络与图论论文/dataset/gowalla/qwynb.txt","w",stdout);

    //freopen("/Users/gjy/Documents/社交网络与图论论文/dataset/foursquare_extend/foursquare_extend.inf","r",stdin);
    //freopen("/Users/gjy/Documents/社交网络与图论论文/dataset/foursquare_extend/qwynb.txt","w",stdout);
    freopen("/Users/gjy/Documents/社交网络与图论论文/dataset/youtube/com-youtube.ungraph-reid.txt", "r", stdin);

    Graph graph;

    graph.InputGraph();
//Get core'K
    //CoresDelLine core_del_line = FindCores(graph);
    //cout << "Core'K="<<core_del_line[core_del_line.size()-1].k_<<endl;
//Get truss'K
    Truss truss = FindTruss(graph);
    int max_truss = 0;
    for (int i = 0; i < graph.m_; i++) max_truss = max(max_truss, truss[i].sup_);
    cout << max_truss << endl;
//Get the number of triangle
    Truss sup_edge = ComputerSup(graph);
    int sum_truss = 0;
    for(int i = 0; i < graph.m_; i++) sum_truss += sup_edge[i].sup_;
    cout<< sum_truss/3 <<endl;

//    CoresDelLine cores_del_line = FindCores(graph);
//    cout<<"Check Cores Del Line : "<<CheckCoresDelLine(graph, cores_del_line)<<endl;

//    string file_str = "/Users/gjy/Documents/社交网络与图论论文/dataset/small/query.in";
//    NeedColorList need_color_vertex_list = InputNeedColorList(file_str);
//
//    Cores cores = FindLeastColorCores(graph, need_color_vertex_list, cores_del_line);
//    cout<<cores.k_<<endl;
//    for(int i = 0; i < cores.vertex_.size(); i++)
//        cout<<(cores.vertex_[i]+1)<<" ";
//    cout<<endl;

//    Truss truss = FindTruss(graph);
//    for(int i = 0; i< truss.size();i++) printf("%d %d %d\n",truss[i].v_+1,truss[i].u_+1,truss[i].sup_);
//    printf("\n");
//    cout<<KCliqueCommunity(graph, 4)<<endl;
//    while(1) {
//        graph.ClearGraph();
//        graph.RandomInputGraph();
//        cout<<"------------------------"<<endl;
//        cout << "Found= "<<KCliqueCommunity(graph, 5) << endl;
//
//    }
    return 0;
}