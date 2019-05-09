#include <iostream>
#include "cores.h"
#include "graph.h"
#include "file_manage.h"

int main() {

    //freopen("/Users/gjy/Documents/社交网络与图论论文/dataset/gowalla/gowalla.inf","r",stdin);
   // freopen("/Users/gjy/Documents/社交网络与图论论文/dataset/gowalla/qwynb.txt","w",stdout);

    //freopen("/Users/gjy/Documents/社交网络与图论论文/dataset/foursquare_extend/foursquare_extend.inf","r",stdin);
    //freopen("/Users/gjy/Documents/社交网络与图论论文/dataset/foursquare_extend/qwynb.txt","w",stdout);
    freopen("/Users/gjy/Documents/社交网络与图论论文/dataset/small/graph.in","r",stdin);
    Graph graph = InputGraph();
    CoresDelLine cores_del_line = FindCores(graph);

    cout<<"Check Cores Del Line : "<<CheckCoresDelLine(graph, cores_del_line)<<endl;
    string file_str = "/Users/gjy/Documents/社交网络与图论论文/dataset/small/query.in";
    NeedColorList need_color_vertex_list = InputNeedColorList(file_str);

    Cores cores = FindLeastColorCores(graph, need_color_vertex_list, cores_del_line);
    cout<<cores.k_<<endl;
    for(int i = 0; i < cores.vertex_.size(); i++)
        cout<<(cores.vertex_[i]+1)<<" ";
    cout<<endl;

    return 0;
}