#include <stdlib.h>

#include <cassert>
#include <iostream>
#include <string>

#include "./problem.h"
#include "./solutions.h"

int main(int argc, char **argv) {
  cryptopals::ProblemManager manager;
  cryptopals::add_solutions(&manager);
  return manager.TestAll() == 0 ? 0 : 1;
}
