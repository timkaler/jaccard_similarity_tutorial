#include <cilk/cilk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <set>
#include "common/graph.h"
#include "common/graphIO.h"
#include "common/graphUtils.h"
#include "common/cxxopts.hpp"


// Subroutine of algorithm_1
float process_vertex_pair(graph<intT> G, int u, int v) {
  intT* u_neighbors = G.V[u].Neighbors;
  int u_degree = G.V[u].degree;

  intT* v_neighbors = G.V[v].Neighbors;
  int v_degree = G.V[v].degree;

  std::set<intT> u_nset;
  std::set<intT> v_nset;

  for (int i = 0; i < u_degree; i++) {
    u_nset.insert(u_neighbors[i]);
  }

  int _intersect = 0;
  int _union = u_degree;
  for (int i = 0; i < v_degree; i++) {
    if (u_nset.find(v_neighbors[i]) != u_nset.end()) {
      // found common neighbor
      _intersect++;
    } else {
      // vertex in union that didn't contribute to u_degree, so increment union counter.
      _union++;
    }
  }

  return 1.0*_intersect/(1.0*_union);
}


// Subroutine of algorithm_2
void process_vertex(graph<intT> G, int u, int* _intersect) {
  intT* u_neighbors = G.V[u].Neighbors;
  int u_degree = G.V[u].degree;

  for (int i = 0; i < u_degree; i++) {
    for (int j = 0; j < u_degree; j++) {
      int u = u_neighbors[i];
      int v = u_neighbors[j];
      _intersect[u*G.n + v] += 1.0;
    }
  }
}


// Complexity O(E\Delta)
float* algorithm_2(graph<intT> G) {
  int* _intersect = (int*) calloc(G.n*G.n, sizeof(int));

  for (int i = 0; i < G.n; i++) {
    process_vertex(G, i, _intersect);
  }

  float* jacard = (float*) malloc(sizeof(float)*G.n*G.n);
  for (int i = 0; i < G.n; i++) {
    for (int j = 0; j < G.n; j++) {
      jacard[i*G.n + j] = (1.0*_intersect[i*G.n + j]) /
          (1.0*(G.V[i].degree + G.V[j].degree - _intersect[i*G.n + j]));
    }
  }
  return jacard;
}


float* algorithm_1(graph<intT> G) {
  float* jacard = (float*) malloc(sizeof(float)*G.n*G.n);
  for (int i = 0; i < G.n; i++) {
    for (int j = 0; j < G.n; j++) {
      jacard[i*G.n + j] = process_vertex_pair(G, i, j);
    }
  }

  return jacard;
}


int main(int argc, char** argv) {
  std::vector<std::string> required_args;
  required_args.push_back("input");

  std::string input_file;

  cxxopts::Options options(argv[0], "Command line options.");
  options.positional_help("Positional help text");
  options.add_options()
    ("i,input", "Input graph", cxxopts::value<std::string>(input_file))
    ("help", "Print help");


  options.parse(argc, argv);
  if (options.count("help")) {
    std::cout << options.help({"", "Group"}) << std::endl;
    return 0;
  }

  bool missing_required = false;
  for (int i = 0; i < required_args.size(); i++) {
    if (options.count(required_args[i]) == 0) {
      printf("Missing required arguments: %s\n", required_args[i].c_str());
      missing_required = true;
    }
  }

  if (missing_required) {
    std::cout << options.help({"", "Group"}) << std::endl;
    return 0;
  }

  graph<intT> G = readGraphFromFile<intT>((char*) input_file.c_str());

  printf("Loaded graph with:\n");
  printf("vertices %d\n", G.n);
  printf("edges %d\n", G.m);

  float* j1 = algorithm_1(G);
  float* j2 = j1;//algorithm_2(G);
  for (int i = 0; i < G.n; i++) {
    for (int j = 0; j < G.n; j++) {
      float val1 = j1[i*G.n + j];
      float val2 = j2[i*G.n + j];
      float delta = val1-val2;
      if (delta*delta > 0.0001) printf("Error! alg1 %f alg2 %f\n", val1, val2);
    }
  }


  return 0;
}
