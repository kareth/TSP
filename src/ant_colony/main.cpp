#include "../common.h"

const double BETA = 3;
const double ALPHA = 1;
const double BASIC_PHEROMONE = 0.5;
const double Q = 1;

const int STEPS = 100;
const int ANTS = 15;

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
    void reinitialize(){
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

int solve(AOGraph &G, vector<int> &path){
  vector<int> bestPath;
  int bestPathLength = 1000000;

  vector<Ant*> ants;

  REP(j, ANTS){
    Ant* newAnt = new Ant;
    ants.push_back(newAnt);
  }

  REP(i, STEPS){

    REP(j, ANTS){
      ants[j]->reinitialize();
    }



    REP(j, G.size-1)
      REP(k, ANTS)
        ants[k]->move();

    int shortestAnt = 0;
    int shortestPathLength;
    vector<int> shortestPath;

    REP(j, ANTS)
      if( ants[shortestAnt]->getPath() > ants[j]->getPath())
        shortestAnt = j;

    shortestPathLength = ants[shortestAnt]->getPath(shortestPath);
    G.updateGlobally(shortestPath, shortestPathLength);

    if( bestPathLength > shortestPathLength){
      bestPath = shortestPath;
      bestPathLength = shortestPathLength;
    }
  }
  path = bestPath;
  return bestPathLength;
}

int main(){
  srand(time(0));
  readData(G);

  vector<int> path;
  int result = solve(G, path);

  printResult(result, path);

  return 0;
}
