#include "./problem.h"

#include <cassert>
#include <iostream>

namespace cryptopals {
Problem *ProblemManager::FindProblem(const std::pair<int, int> &number) {
  for (const auto &p : problems_) {
    if (p->number() == number) {
      return p.get();
    }
  }
  return nullptr;
}

void ProblemManager::AddProblem(std::unique_ptr<Problem> p) {
  assert(p != nullptr);
  assert(FindProblem(p->number()) == nullptr);
  problems_.push_back(std::move(p));
}

int ProblemManager::TestAll() {
  int fails = 0;
  for (auto &p : problems_) {
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
