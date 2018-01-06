// Copyright (c) 2018 Evan Klitzke <evan@eklitzke.org>
//
// This file is part of cryptopals.
//
// cryptopals is free software: you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free Software
// Foundation, either version 3 of the License, or (at your option) any later
// version.
//
// cryptopals is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
// A PARTICULAR PURPOSE. See the GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along with
// cryptopals. If not, see <http://www.gnu.org/licenses/>.

#include "./problem.h"

#include <cassert>
#include <iostream>

#include "./buffer.h"
#include "./solutions.h"

namespace cryptopals {

inline int run_problem(int x, int y, func f) {
  std::cout << x << "." << y << " " << std::flush;
  if (f()) {
    std::cout << "OK" << std::endl;
    return 0;
  }
  std::cout << "FAIL" << std::endl;
  return 1;
}

ProblemManager::ProblemManager() { add_all_solutions(this); }

void ProblemManager::AddSolution(int x, int y, func f) {
  std::pair<int, int> pr{x, y};
  if (solutions_.find(pr) != solutions_.end()) {
    std::cerr << "Duplicate solution " << x << "." << y << std::endl;
    assert(false);
  }
  solutions_.emplace(std::make_pair(pr, f));
}

int ProblemManager::TestAll() {
  int fails = 0;
  for (auto &kv : solutions_) {
    fails += run_problem(kv.first.first, kv.first.second, kv.second);
  }
  return fails;
}

int ProblemManager::TestSet(int set) {
  int fails = 0;
  bool found = false;
  for (auto &kv : solutions_) {
    if (kv.first.first == set) {
      found = true;
      fails += run_problem(kv.first.first, kv.first.second, kv.second);
    }
  }
  if (!found) {
    std::cerr << "set not found\n";
    return 1;
  }
  return fails;
}

int ProblemManager::TestProblem(int set, int problem) {
  auto it = solutions_.find({set, problem});
  if (it == solutions_.end()) {
    std::cerr << "problem not found\n";
    return 1;
  }
  return run_problem(set, problem, it->second);
}
}  // namespace cryptopals
