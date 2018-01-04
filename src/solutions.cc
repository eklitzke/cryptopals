#include "./solutions.h"

#define SOLUTION(x, y) manager->AddProblem(new Problem_##x##_##y);

namespace cryptopals {
void add_solutions(ProblemManager *manager) {
  SOLUTION(1, 1);
  SOLUTION(1, 2);
}
}  // namespace cryptopals
