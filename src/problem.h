// Copyright (c) 2017 Evan Klitzke <evan@eklitzke.org>
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
