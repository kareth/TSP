#include "../common.h"

#define POPULATION_SIZE 100
#define TOP_POOL        50
#define EVOLUTION_TIMES 100000

class Genome {
  public:
    Graph G;
    vector<int> genes;
    int score;

    Genome(vector<int> &path, Graph &G) {
      this->genes = vector<int>(path);
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
      set<int> used;
      for(int i = 0; i < genes.size(); i++) {
        if(genes[i] == genome.genes[i]) {
          result.push_back(genes[i]);
        }
        else {
          int insert;
          if(used.find(genes[i]) == used.end()) insert = genes[i];
          else insert = genome.genes[i];
          used.insert(insert);
          result.push_back(insert);
        }
      }
      Genome new_genome(result, G);
      return new_genome.mutate();
      /*
      if(score < genome.score) {
        Genome new_genome(genes, G);
        return new_genome.mutate();
      }
      else {
        Genome new_genome(genome.genes, G);
        return new_genome.mutate();
      }
      */
    }

  private:
    int path_length() {
      int sum = 0;
      REP(i, genes.size()) sum += G.distance[genes[i]][genes[(i+1)%G.size]];
      return sum;
    }
};

bool genome_compare(Genome g1, Genome g2) {
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
    for(int l = 0; l < TOP_POOL; l++) {
      int r1 = rand() % TOP_POOL;
      int r2 = rand() % TOP_POOL;
      population.push_back(population[r1].breed(population[r2]));
    }
    sort(population.begin(), population.end(), genome_compare);
    population.erase(population.begin() + POPULATION_SIZE, population.end());
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
