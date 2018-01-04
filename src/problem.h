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
  void AddSolution(int x, int y, std::unique_ptr<Problem> p);

  // Test everything, and return the number of failures.
  int TestAll();

 private:
  std::map<std::pair<int, int>, std::unique_ptr<Problem> > problems_;

  Problem *FindProblem(const std::pair<int, int> &number);
};
}  // namespace cryptopals
