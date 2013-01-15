#include "../common.h"

const double BETA = 1;
const double ALPHA = 0.5;
const double BASIC_PHEROMONE = 0.0001;
const double Q = 0.5;

const int STEPS = 50;
const int ANTS = 50;

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
    }

    void updateGlobally(vector<int> &path, int length){
      REP(i, path.size()-1){
        int x = path[i], y = path[i+1];
        pheromone[x][y] = (1.0 - ALPHA) * pheromone[x][y] + ALPHA / double(length);
      }
    }
};

AOGraph G;


class Ant{
  vector<int> visited;
  int distanceTravelled;


  private:
    bool isVisited(int v){
      return find(visited.begin(), visited.end(), v) != visited.end();
    }

    int lastCity(){return visited.back();}

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
      return i;
    }

    int getScores(vector<double> &scores){
      int sum = 0.0;
      REP(i, G.size){
        if(!isVisited(i)){
          int score = G.score(lastCity(), i);
          scores.push_back(score);
          sum += score;
        }
      }
      return sum;
    }

  public:
    Ant(){
      visited.push_back(rand()%G.size);
    }

    void move(){
      int next;
      if (rand01() < Q) next = moveToBest();
      else next = moveRandomly();

      G.updateLocalTrial(lastCity(), next);
    }

    int getPath(){ return distanceTravelled;}
    int getPath(vector<int> &v){ v = visited; return distanceTravelled;}
};

int solve(AOGraph &G, vector<int> &path){
  vector<int> bestPath;
  int bestPathLength = 1000000000;
  REP(i, STEPS){
    vector<Ant> ants;
    REP(j, ANTS){
      Ant newAnt;
      ants.push_back(newAnt);
    }

    REP(j, G.size-1)
      REP(k, ANTS)
        ants[k].move();

    int shortestAnt = 0;
    int shortestPathLength;
    vector<int> shortestPath;

    REP(j, ANTS)
      if( ants[shortestAnt].getPath() < ants[j].getPath())
        shortestAnt = j;

    shortestPathLength = ants[shortestAnt].getPath(shortestPath);
    G.updateGlobally(shortestPath, shortestPathLength);
  }
}

int main(){
  srand(time(0));
  readData(G);

  vector<int> path;
  int result = solve(G, path);

  printResult(result, path);

  return 0;
}
