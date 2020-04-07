//
// Created by Jiyang Gong on 2019-03-26.
//

#include "include.h"

void read(int &x)
{
    int f=1;x=0;char s=getchar();
    while(s<'0'||s>'9'){if(s=='-')f=-1;s=getchar();}
    while(s>='0'&&s<='9'){x=x*10+s-'0';s=getchar();}
    x*=f;

}

char* StrToChar(string str){
    char file_s[100] = "";
    strcpy(file_s, str.c_str());
    return file_s;
}

int ran(int md){
    return rand() % md + 1;
}
void InputNodeToString(map<int, string> &author_map){
    ifstream fin;
    fin.open("/Users/gjy/Documents/社交网络与图论论文/dataset/small/author_name.txt");
    int author_id, author_name;
    while(fin>>author_id>>author_name){
        author_map[author_id] = author_name;
    }
}
void ReidGraph() {
    FILE *out_fp = fopen("/Users/gjy/Documents/社交网络与图论论文/dataset/facebook/facebook-combined-reid.txt", "w");
    int n, m;
    read(n);
    read(m);
    cout << n << " " << m << endl;

    vector<int> id_edge;
    vector<pair<int, int>> in_edge;
    for (int i = 0; i < m; i++) {
        int x, y;
        read(x);
        read(y);
        in_edge.push_back(make_pair(x, y));
        id_edge.push_back(x);
        id_edge.push_back(y);
    }
    sort(in_edge.begin(), in_edge.end());
    sort(id_edge.begin(), id_edge.end());
    id_edge.erase(unique(id_edge.begin(), id_edge.end()), id_edge.end());
    assert(id_edge.size() <= n);


    for (auto &edge:in_edge) {
        int &x = edge.first, &y = edge.second;
        x = lower_bound(id_edge.begin(), id_edge.end(), x) - id_edge.begin() + 1;
        y = lower_bound(id_edge.begin(), id_edge.end(), y) - id_edge.begin() + 1;
    }

    assert(in_edge.size() == m);
    fprintf(out_fp, "%d %d\n", n, m);
    for (auto edge:in_edge) {
        fprintf(out_fp, "%d %d\n", edge.first, edge.second);
    }
    fclose(out_fp);
}