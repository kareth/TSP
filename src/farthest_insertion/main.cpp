#include "../common.h"

class CNGraph : public Graph{
};


int solve(CNGraph &G, vector<int> &path){
  // TODO: insert smart stuff here
  return 0;
}

int main(){
  CNGraph G;

  readData(G);

  vector<int> path;
  int result = solve(G, path);

  printResult(result, path);

  return 0;
}
