#include "./problem.h"

#include <cassert>
#include <iostream>

namespace cryptopals {
Problem *ProblemManager::FindProblem(const std::pair<int, int> &number) {
  for (const auto &p : problems_) {
    if (p->number() == number) {
      return p;
    }
  }
  return nullptr;
}

void ProblemManager::AddProblem(Problem *p) {
  assert(FindProblem(p->number()) == nullptr);
  problems_.push_back(p);
}

int ProblemManager::TestAll() {
  int fails = 0;
  for (Problem *p : problems_) {
    const std::pair<int, int> pr = p->number();
    std::cout << pr.first << "." << pr.second << " " << std::flush;
    bool ok = p->test();
    if (ok) {
      std::cout << "OK" << std::endl;
    } else {
      std::cout << "FAIL" << std::endl;
      fails++;
    }
  }
  return fails;
}
}  // namespace cryptopals
