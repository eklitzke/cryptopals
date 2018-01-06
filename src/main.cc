// Copyright (c) 2018 Evan Klitzke <evan@eklitzke.org>
//
// This file is part of cryptopals.
//
// cryptopals is free software: you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free Software
// Foundation, either version 3 of the License, or (at your option) any later
// version.
//
// cryptopals is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
// A PARTICULAR PURPOSE. See the GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along with
// cryptopals. If not, see <http://www.gnu.org/licenses/>.

#include <getopt.h>
#include <stdlib.h>

#include <cassert>
#include <iostream>
#include <string>

#include "./problem.h"

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
