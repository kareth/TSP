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

    void init(int graphSize){
      size = graphSize;
      distance.resize(size);
      REP(i, size) distance[i].resize(size);
      return;
    }

    void addEdge(int x, int y, int length){ distance[x][y] = length;}

};

class CNGraph : public Graph{
  public:
    vector<int> visited;

    void init(int graphSize){
      Graph::init(graphSize);
      visited.resize(size);
    }
    int closestNotVisited(int v){
      int closest = v;
      REP(i, size){
        if( i == v || isVisited(i)) continue;
        if( closest == v) closest = i;

        if( distance[v][i] < distance[v][closest])
          closest = i;
      }
      return closest;
    }

    void visit(int v){ visited[v] = 1;}
    int isVisited(int v){ return visited[v];}
};


// This function could also be part of graph class
void readData(CNGraph &G){
  int size, dist;
  scanf("%d",&size);
  G.init(size);

  REP(i, G.size)
    REP(j, G.size){
      scanf("%d",&dist);
      G.addEdge(i, j, dist);
    }
  return;
}


// TODO Refactor
// This whole function smells and should be partially incorporated into CNGraph.
int solve(CNGraph &G, vector<int> &path){
  int startingVerticle = 0; // We can think of any way of improving that, becouse other starting points will give other results

  path.push_back(startingVerticle);
  G.visit(path.back());

  while(path.size() < G.size){
    path.push_back(G.closestNotVisited(path.back()));
    G.visit(path.back());
  }

  int sum = 0;
  REP(i, path.size()) sum += G.distance[path[i]][path[(i+1)%G.size]];

  return sum;
}

void printResult(int result, vector<int> &path){
  printf("%d\n",result);
  REP(i, path.size()) printf("%d ",path[i]);
  printf("\n");
}




int main(){
  CNGraph G;

  readData(G);

  vector<int> path;
  int result = solve(G, path);

  printResult(result, path);

  return 0;
}
