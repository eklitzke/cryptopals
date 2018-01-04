#pragma once

#include <map>
#include <memory>
#include <utility>

namespace cryptopals {
class ProblemManager;

struct Problem {
  // Run the test case, returns true on success and false on failure.
  virtual bool test() = 0;
};

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
  std::map<std::pair<int, int>, std::unique_ptr<Problem> > problems_;

  void AddSolution(int x, int y, std::unique_ptr<Problem> p);
};
}  // namespace cryptopals
