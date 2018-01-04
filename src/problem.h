#pragma once

#include <functional>
#include <map>
#include <utility>

namespace cryptopals {

typedef std::function<bool(void)> func;

class ProblemManager {
 public:
  ProblemManager();

  // Test everything, and return the number of failures.
  int TestAll();

  // Test a given problem set.
  int TestSet(int set);

  // Test a given problem.
  int TestProblem(int set, int problem);

 private:
  std::map<std::pair<int, int>, func> solutions_;

  void AddSolution(int x, int y, std::function<bool(void)> func);
};
}  // namespace cryptopals
