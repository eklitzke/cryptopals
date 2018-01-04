#pragma once

#include <utility>
#include <vector>

namespace cryptopals {
class ProblemManager;

struct Problem {
  // Return the case/problem number.
  virtual std::pair<int, int> number() = 0;

  // Run the test case, returns true on success and false on failure.
  virtual bool test() = 0;
};

class ProblemManager {
 public:
  void AddProblem(Problem *p);

  // Test everything, and return the number of failures.
  int TestAll();

 private:
  std::vector<Problem *> problems_;

  Problem *FindProblem(const std::pair<int, int> &number);
};
}  // namespace cryptopals
