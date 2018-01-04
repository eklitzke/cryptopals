#pragma once

#include <memory>
#include <utility>
#include <vector>

namespace cryptopals {
class ProblemManager;

struct Problem {
  // Return the case/problem number.
  virtual std::pair<int, int> number() const = 0;

  // Run the test case, returns true on success and false on failure.
  virtual bool test() = 0;
};

class ProblemManager {
 public:
  void AddProblem(std::unique_ptr<Problem> p);

  // Test everything, and return the number of failures.
  int TestAll();

 private:
  std::vector<std::unique_ptr<Problem> > problems_;

  Problem *FindProblem(const std::pair<int, int> &number);
};
}  // namespace cryptopals
