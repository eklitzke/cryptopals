#include "./problem.h"

#include <cassert>
#include <iostream>
#include <utility>

namespace cryptopals {

inline int run_problem(int x, int y, Problem *p) {
  std::cout << x << "." << y << " " << std::flush;
  if (p->test()) {
    std::cout << "OK" << std::endl;
    return 0;
  }
  std::cout << "FAIL" << std::endl;
  return 1;
}

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
    fails += run_problem(kv.first.first, kv.first.second, kv.second.get());
  }
  return fails;
}

int ProblemManager::TestSet(int set) {
  int fails = 0;
  bool found = false;
  for (auto &kv : problems_) {
    if (kv.first.first == set) {
      found = true;
      fails += run_problem(kv.first.first, kv.first.second, kv.second.get());
    }
  }
  if (!found) {
    std::cerr << "set not found\n";
    return 1;
  }
  return fails;
}

int ProblemManager::TestProblem(int set, int problem) {
  for (auto &kv : problems_) {
    if (kv.first.first == set && kv.first.second == problem) {
      return run_problem(kv.first.first, kv.first.second, kv.second.get());
    }
  }
  std::cerr << "problem not found\n";
  return 1;
}
}  // namespace cryptopals
