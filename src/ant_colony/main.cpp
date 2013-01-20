#include "../common.h"

const double BETA = 1;
const double ALPHA = 0.5;
const double BASIC_PHEROMONE = 0.001;
const double Q = 0.97;

const int STEPS = 200;
const int ANTS = 8;

double rand01(){
  return double(rand()) / double(RAND_MAX);
}

class AOGraph : public Graph{
  public:
    vector<vector<double> > pheromone;

    void init(int graphSize){
      Graph::init(graphSize);
      pheromone.resize(size);
      REP(i, size)pheromone[i].resize(size);
    }

    double score(int x, int y){
      return pheromone[x][y] * pow(1.0/distance[x][y], BETA);
    }

    void updateLocalTrial(int x, int y){
      pheromone[x][y] = (1.0 - ALPHA)* pheromone[x][y] + ALPHA * BASIC_PHEROMONE;
      pheromone[y][x] = pheromone[x][y];
    }

    void updateGlobally(vector<int> &path, int length){
      REP(i, path.size()-1){
        int x = path[i], y = path[i+1];
        pheromone[x][y] = (1.0 - ALPHA) * pheromone[x][y] + ALPHA / double(length);
        pheromone[y][x] = pheromone[x][y];
      }
    }
};

AOGraph G;


class Ant{
  int distanceTravelled;
  vector<int> path;
  vector<int> visited;

  private:
    bool isVisited(int v){
      return visited[v];
    }


    int lastCity(){return path.back();}


    int moveToBest(){
      int best = lastCity();
      REP(i, G.size){
        if ( !isVisited(i) ){
          if( best == lastCity()) best = i;
          if( G.score(lastCity(), best) < G.score(lastCity(), i))
            best = i;
        }
      }
      return best;
    }


    int moveRandomly(){
      vector<double> scores;
      double sum = 0;
      sum = getScores(scores);
      return getRandom(scores, sum);
    }


    int getRandom(vector<double> scores, double sum){
      double r = rand01()*sum;
      int i;
      for(i = 0; i < scores.size() && r > 0; i++){
        r -= G.score(lastCity(), scores[i]);
      }
      return scores[i];
    }


    int getScores(vector<double> &scores){
      int sum = 0.0;
      REP(i, G.size){
        if(!isVisited(i)){
          int score = G.score(lastCity(), i);
          scores.push_back(i);
          sum += score;
        }
      }
      return sum;
    }


    void addToPath(int v){
      path.push_back(v);
      visited[v] = 1;
    }


  public:
    void reset(){
      path.clear();
      distanceTravelled = 0;
      REP(i, visited.size()) visited[i] = 0;
      addToPath(rand()%G.size);
    }


    Ant(){
      visited.resize(G.size);
      addToPath(rand()%G.size);
    }


    void move(){
      int next;
      if (rand01() < Q) next = moveToBest();
      else next = moveRandomly();

      G.updateLocalTrial(lastCity(), next);
      distanceTravelled += G.distance[lastCity()][next];
      addToPath(next);
    }


    int getPath(){ return distanceTravelled + G.distance[path.back()][path[0]];}
    int getPath(vector<int> &v){ v = path; return distanceTravelled + G.distance[path.back()][path[0]];}


    void printPath(){
      printf("Path: ");
      REP(i, path.size()) printf("%d ",path[i]);
      printf("\n");
    }
};



void runStep(vector<Ant*> &ants, int &sPathLen, vector<int> &sPath){
  REP(j, ANTS)
    ants[j]->reset();

  REP(j, G.size-1)
    REP(k, ANTS)
      ants[k]->move();

  int bAnt = 0;

  REP(j, ANTS)
    if( ants[bAnt]->getPath() > ants[j]->getPath())
      bAnt = j;

  sPathLen = ants[bAnt]->getPath(sPath);
  G.updateGlobally(sPath, sPathLen);
}



int solve(AOGraph &G, vector<int> &bPath){
  vector<Ant*> ants;

  REP(j, ANTS){
    Ant* newAnt = new Ant;
    ants.push_back(newAnt);
  }

  vector<int> sPath;
  int bPathLen = 1000000, sPathLen;

  REP(i, STEPS){
    runStep(ants, sPathLen, sPath);

    if(sPathLen < bPathLen){
      bPathLen = sPathLen;
      //bPath = sPath;
    }
  }
  return bPathLen;
}



int main(){
  srand(time(0));
  readData(G);


  int res = 1000000000;
  REP(i, 1){
    vector<int> path;
    int result = solve(G, path);
    res = min(res, result);
    //printResult(result, path);
    //printf("%d\n",res);
  }
  printf("%d\n",res);
  return 0;
}
