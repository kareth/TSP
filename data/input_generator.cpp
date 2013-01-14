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
using namespace std;

const int forbiddenSize = 10000;

void init(){
  srand(time(0));
}

// Returns random value from [0...maxValue]
int random(int maxValue){ return rand() % maxValue; }


// Prints input data to stdout
void printInput(int graphSize, int maxValue){

  // Print number of verticles in the graph
  printf("%d\n",graphSize);

  // Print incidence matrix
  REP(i, graphSize){
    REP(j, graphSize)
      printf("%d ", random(maxValue));
    printf("\n");
  }

  return;
}

int main(){
  init();

  // TODO take those values as parameters
  const int graphSize = 1000;
  const int maxValue = 1000;

  printInput(graphSize, maxValue);

  return 0;
}
