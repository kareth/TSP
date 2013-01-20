#include "../common.h"

class CNGraph : public Graph {
  public:
  int path_size(vector<int> &path) {
    int sum = 0;
    REP(i, path.size()) sum += distance[path[i]][path[(i+1)%size]];
    return sum;
  }
};

void heap_permute(int n, vector<int> &current, vector<vector<int> > &results) {
  if(n == 1) {
    results.push_back(vector<int>(current));
  }
  else {
    for(int i = 0; i < n; i++) {
      heap_permute(n-1, current, results);
      if(n % 2 == 0) swap(current[0], current[n-1]);
      else swap(current[i], current[n-1]);
    }
  }
}

int solve(CNGraph &G, vector<int> &path){
  vector<vector<int> > results;
  vector<int> current;
  REP(i, G.size) current.push_back(i);
  heap_permute(G.size, current, results);

  int min = INF, minno = 0;
  for(int i = 0; i < results.size(); i++) {
    int tmp = G.path_size(results[i]);
    if(tmp < min) {
      min = tmp;
      minno = i;
    }
  }

  path.clear();
  REP(i, G.size) path.push_back(results[minno][i]);
  return min;
}

int main(){
  CNGraph G;

  readData(G);

  vector<int> path;
  int result = solve(G, path);

  printResult(result, path);

  return 0;
}
