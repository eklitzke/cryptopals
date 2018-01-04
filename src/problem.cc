#include "./problem.h"

#include <cassert>
#include <iostream>
#include <utility>

namespace cryptopals {
void ProblemManager::AddSolution(int x, int y, std::unique_ptr<Problem> p) {
  std::pair<int, int> pr{x, y};
  assert(p != nullptr);
  if (problems_.find(pr) != problems_.end()) {
    std::cerr << "Duplicate solution " << x << "." << y << std::endl;
    assert(false);
  }
  problems_.emplace(std::make_pair(pr, std::move(p)));
}

int ProblemManager::TestAll() {
  int fails = 0;
  for (auto &kv : problems_) {
    std::cout << kv.first.first << "." << kv.first.second << " " << std::flush;
    bool ok = kv.second->test();
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
