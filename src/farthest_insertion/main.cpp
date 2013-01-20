#include "../common.h"

vector<int> global;

bool f_compare(int i, int j) {
  return (global[i] < global[j]);
}

void farthest_sort(vector<int> &v) {
  sort(v.begin(), v.end(), f_compare);
}

class CNGraph : public Graph{
  public:
    vector<int> visited;
    vector<vector<int> > farthest;
    vector<int> farthestPointer;

    void init(int graphSize){
      Graph::init(graphSize);
      visited.resize(size);
      farthest.resize(size);
      farthestPointer.resize(size);
    }

    void visit(int v){ visited[v] = 1;}
    int isVisited(int v){ return visited[v];}

    void prepareFarthestData() {
      for(int i = 0; i < size; i++) {
        farthestPointer[i] = size-1;
        for(int l = 0; l < size; l++) {
          farthest[i].push_back(l);
        }
        global = distance[i];
        farthest_sort(farthest[i]);
      }
    }

    int farthestFreeVerticle() {
      int result = -1, max = -INF;
      for(int i = 0; i < size; i++) {
        if(isVisited(i)) {
          int verticle = farthest[i][farthestPointer[i]];
          while(isVisited(verticle)) {
            farthestPointer[i]--;
            verticle = farthest[i][farthestPointer[i]];
          }
          if(max < distance[i][verticle]) {
            max = distance[i][verticle];
            result = verticle;
          }
        }
      }
      return result;
    }

    list<int>::iterator closestVerticleArc(list<int> &path, int verticle) {
      list<int>::iterator result = path.begin();
      int min = INF, arc1, arc2, arclen;
      for(list<int>::iterator i = path.begin(); i != path.end(); ++i) {
        arc1 = *i;
        list<int>::iterator next = i;
        ++next;
        if(next==path.end())
          arc2 = *(path.begin());
        else
          arc2 = *next;
        arclen = distance[arc1][verticle] + distance[verticle][arc2] - distance[arc1][arc2];
        if(arclen < min) {
          result = i;
          min = arclen;
        }
      }
      return result;
    }
};

int solve(CNGraph &G, vector<int> &path){
  G.prepareFarthestData();
  int startingVerticle = 0;
  G.visit(startingVerticle);
  list<int> pathList;
  pathList.push_back(startingVerticle);
  for(int z = 1; z < G.size; z++) {
    int newVerticle = G.farthestFreeVerticle();
    G.visit(newVerticle);
    list<int>::iterator it = G.closestVerticleArc(pathList, newVerticle);
    pathList.insert(++it, newVerticle);
  }
  for(list<int>::iterator i = pathList.begin(); i != pathList.end(); ++i) {
    path.push_back(*i);
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
