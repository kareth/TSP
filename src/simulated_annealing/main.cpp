#include "../common.h"

const int iterations = 10;
const int L = 8000;
const int STEPS = 150;

class SAGraph : public Graph{
  public:

    int pathLength(vector<int> &path){
      int res = 0;

      REP(i, size){
        res += distance[ path[i] ][ path[(i+1) % size] ];
      }
      return res;
    }


    vector<int> getNextPath(vector<int> &v) {
      int a, b;
      a = b = rand() % size;

      while (a == b)
        b = rand() % size;

      vector<int> result = v;

      swap(result[a], result[b]);
      return result;
    }

};


double magicFunction(int d, double temp) {
	return exp((double)(-d) / temp);
}

double alpha(double temp) {
	const double b = 0.0002005;
	return temp / (1 + b * temp);
}


int solve(SAGraph &G, vector<int> &path){
  vector<int> v;
  REP(i, G.size) v.push_back(i);
  random_shuffle(v.begin(), v.end());

  int e = G.pathLength(v);

  vector<int> vn, vb = v;
  int en, eb = e;

  double temp = 10000000000000000;


  REP(i, STEPS){
    REP(i, L){
      vn = G.getNextPath(v);
      en = G.pathLength(vn);

      if(en < eb){ eb = en; vb = vn; }
      int d = en - e;
      if(d < 0){ e = en; v = vn; }
      else{
        double x = (double) rand() / RAND_MAX; // Rand (0..1)
        if(x < magicFunction(d, temp)){
            v = vn;
            e = en;
        }
      }
    }
    temp = alpha(temp);
  }
  path = vb;
  return eb;
}


int main(){
  srand(time(0));
  SAGraph G;
  readData(G);

  int res = 100000000;
  vector<int> respath;

  REP(i, iterations){
    vector<int> path;
    int result = solve(G, path);
    if( result < res){
      res = result;
      respath = path;
    }
  }

  printResult(res, respath);

  return 0;
}

