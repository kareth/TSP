#include<algorithm>
#include<cstdio>
#include<vector>
#include<cmath>
#include<cstring>

#define INF 2000000000
#define REP(i,n) for(int i = 0; i < (n); i++)
#define FOR(i, a, b) for(int i = (a); i < (b); i++)
#define FORD(i, a, b) for(int i = (a); i >= (b); i--)
#define PI pair<int, int>
#define ST first
#define ND second
#define CLR(a, b) memset(a, b, sizeof(a))

#ifdef DEBUG
  #define DBG printf
#else
  #define DBG
#endif

using namespace std;

class Graph{
  public:
    int size;
    vector< vector<int> > distance;

    virtual void init(int graphSize){
      size = graphSize;
      distance.resize(size);
      REP(i, size) distance[i].resize(size);
      return;
    }

    void addEdge(int x, int y, int length){ distance[x][y] = length;}

};

void readData(Graph &G){
  int size, dist;
  scanf("%d",&size);
  G.init(size);

  REP(i, G.size)
    REP(j, G.size){
      scanf("%d",&dist);
      G.addEdge(i, j, dist);
    }
}

void printResult(int result, vector<int> &path){
  printf("%d\n",result);
  //REP(i, path.size()) printf("%d ",path[i]);
  //printf("\n");
}

