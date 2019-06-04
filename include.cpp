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
void ReidGraph(){
    FILE *out_fp = fopen("/Users/gjy/Documents/社交网络与图论论文/dataset/youtube/com-youtube.ungraph-reid.txt","w");
    int n,m;
    read(n);read(m);
    cout<<n<<" " <<m<<endl;
    map<int, int> M;
    int Mid = 0;
    vector<pair<int, int>> edge;
    for(int i = 0 ; i < m; i++) {
        int x,y;
        read(x);read(y);

        if(M.find(x) == M.end()){
            M[x] = ++Mid;
        }
        if(M.find(y) == M.end()){
            M[y] = ++Mid;
        }
        edge.push_back(make_pair(M[x], M[y]));
    }
    assert(Mid == n);
    assert(edge.size() == m);
    fprintf(out_fp, "%d %d\n",n , m);
    for(auto edge:edge){
        fprintf(out_fp, "%d %d\n",edge.first , edge.second);
    }
    fclose(out_fp);
}