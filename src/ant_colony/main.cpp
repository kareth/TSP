#include "../common.h"

class CNGraph : public Graph{
  public:
    vector<int> visited;

    void init(int graphSize){
      Graph::init(graphSize);
      visited.resize(size);
    }
    int closestNotVisited(int v){

      // TODO This selection function needs to be changed
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

int main(){
  CNGraph G;

  readData(G);

  vector<int> path;
  int result = solve(G, path);

  printResult(result, path);

  return 0;
}
