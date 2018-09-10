#include <cilk/cilk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>

#include "common/graph.h"
#include "common/graphIO.h"
#include "common/graphUtils.h"
#include "common/cxxopts.hpp"


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
    return 0;
  }

  graph<intT> G = readGraphFromFile<intT>((char*) input_file.c_str());

  printf("Loaded graph with:\n");
  printf("vertices %d\n", G.n);
  printf("edges %d\n", G.m);

  return 0;
}
