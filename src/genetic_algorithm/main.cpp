#include "../common.h"

#define POPULATION_SIZE 1000
#define EVOLUTION_TIMES 1000

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

    bool valid() {
      set<int> u;
      REP(i, genes.size()) u.insert(genes[i]);
      return u.size() == genes.size();
    }

    Genome mutate() {
      vector<int> result(genes);
      if(rand() % 10 < 6) {
        int random = rand() % result.size();
        swap(result[random], result[(random+1)%result.size()]);
      }
      else {
        int first = rand() % genes.size();
        int second = first + rand() % (genes.size()-first);
        reverse(result.begin()+first, result.begin()+second+1);
      }
      Genome new_genome(result, G);
      return new_genome;
    }

    void breed(Genome &genome, vector<Genome> &population) {
      vector<int> r1, r2;
      set<int> u1, u2;
      r1.resize(genes.size());
      r2.resize(genes.size());

      int co = rand() % genes.size();
      REP(i, co) r1[i] = genes[i];
      REP(i, co) r2[i] = genome.genes[i];
      FOR(i, co, genes.size()) u1.insert(genes[i]);
      FOR(i, co, genes.size()) u2.insert(genome.genes[i]);

      for(int i = co; i < genes.size(); i++) {
          int insert;
          if(u1.find(genome.genes[i]) != u1.end()) {
            insert = genome.genes[i];
          }
          else {
            insert = *u1.begin();
          }
          u1.erase(insert);
          r1[i] = insert;

          if(u2.find(genes[i]) != u2.end()) insert = genes[i];
          else insert = *u2.begin();
          u2.erase(insert);
          r2[i] = insert;
      }

      Genome g1(r1, G), g2(r2, G);
      population.push_back(g1.mutate());
      population.push_back(g2.mutate());
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
    for(int l = 0; l < POPULATION_SIZE / 2; l++) {
      int r1 = rand() % POPULATION_SIZE;
      int r2 = rand() % POPULATION_SIZE;
      while(r1 == r2) r2 = rand() % POPULATION_SIZE;
      population[r1].breed(population[r2], population);
    }
    sort(population.begin(), population.end(), genome_compare);
    population.erase(population.begin() + POPULATION_SIZE, population.end());
    printf("evolution %i: top: %d last: %d\n", i, population[0].score, population[POPULATION_SIZE-1].score);
    //printResult(0, top_genome.genes);
    //printResult(0, population[POPULATION_SIZE-1].genes);
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
