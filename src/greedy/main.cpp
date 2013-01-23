#include "../common.h"

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


int solve(CNGraph &G, vector<int> &path){
  srand(time(0));
  int startingVerticle = rand() % G.size;

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

int main(){
  CNGraph G;

  readData(G);

  vector<int> path;
  int result = solve(G, path);

  printResult(result, path);

  return 0;
}
