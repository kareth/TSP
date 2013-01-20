#include "../common.h"

#define POPULATION_SIZE 1000
#define EVOLUTION_TIMES 500

class Genome {
  public:
    Graph G;
    vector<int> genes;
    int score;

    Genome(vector<int> path, Graph &G) {
      this->genes = path;
      this->G = G;
      this->score = path_length();
    }

    Genome mutate() {
      vector<int> result(genes);
      int random = rand() % result.size();
      swap(result[random], result[(random+1)%result.size()]);
      Genome new_genome(result, G);
      return new_genome;
    }

    Genome breed(Genome &genome) {
      vector<int> result;
      result.resize(genes.size());
      set<int> used;
      int co = rand() % genes.size();
      REP(i, co) result[i] = genes[i];
      REP(i, co) used.insert(result[i]);
      for(int i = co; i < genes.size(); i++) {
          int insert = 0;
          if(used.find(genes[i]) == used.end()) insert = genes[i];
          else if(used.find(genome.genes[i]) == used.end()) insert = genome.genes[i];
          else {
            while(used.find(insert) != used.end()) insert++;
          }
          used.insert(insert);
          result[i] = insert;
      }
      Genome new_genome(result, G);
      return new_genome.mutate();
    }

  private:
    int path_length() {
      int sum = 0;
      REP(i, genes.size()) sum += G.distance[genes[i]][genes[(i+1)%G.size]];
      return sum;
    }
};

bool genome_compare(const Genome &g1, const Genome &g2) {
  return (g1.score < g2.score);
}

int solve(Graph &G, vector<int> &path){
  vector<Genome> population;
  REP(i, G.size) path.push_back(i);
  for(int i = 0; i < POPULATION_SIZE; i++) {
    random_shuffle(path.begin(), path.end());
    Genome new_genome(path, G);
    population.push_back(new_genome);
  }
  sort(population.begin(), population.end(), genome_compare);

  for(int i = 0; i < EVOLUTION_TIMES; i++) {
    for(int l = 0; l < POPULATION_SIZE; l++) {
      int r1 = rand() % POPULATION_SIZE;
      int r2 = rand() % POPULATION_SIZE;
      population.push_back(population[r1].breed(population[r2]));
    }
    sort(population.begin(), population.end(), genome_compare);
    population.erase(population.begin() + POPULATION_SIZE, population.end());
    //printf("evolution %i: top: %d last: %d\n", i, population[0].score, population[POPULATION_SIZE-1].score);
  }

  path.clear();
  REP(i, G.size) path.push_back(population[0].genes[i]);
  return population[0].score;
}

int main(){
  Graph G;
  srand(time(0));

  readData(G);

  vector<int> path;
  int result = solve(G, path);

  printResult(result, path);

  return 0;
}
