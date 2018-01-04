#include <getopt.h>
#include <stdlib.h>

#include <cassert>
#include <iostream>
#include <string>

#include "./problem.h"
#include "./solutions.h"

inline int retval(int val) { return val == 0 ? 0 : 1; }

int main(int argc, char **argv) {
  static const char short_opts[] = "h";
  static struct option long_opts[] = {{"help", no_argument, 0, 'h'},
                                      {0, 0, 0, 0}};
  for (;;) {
    int c = getopt_long(argc, argv, short_opts, long_opts, nullptr);
    if (c == -1) {
      break;
    }
    switch (c) {
      case 'h':
        std::cout << "usage: " << argv[0] << "\n";
        return 0;
        break;
      case '?':
        // getopt_long should already have printed an error message
        break;
      default:
        std::cerr << "unrecognized command line flag: " << optarg << "\n";
        abort();
    }
  }
  cryptopals::ProblemManager manager;
  cryptopals::add_solutions(&manager);
  if (argc - optind == 1) {
    unsigned long int set = std::strtoul(argv[optind], nullptr, 10);
    return retval(manager.TestSet(set));
  } else if (argc - optind == 2) {
    unsigned long int set = std::strtoul(argv[optind++], nullptr, 10);
    unsigned long int problem = std::strtoul(argv[optind], nullptr, 10);
    return retval(manager.TestProblem(set, problem));
  }
  return retval(manager.TestAll());
}
