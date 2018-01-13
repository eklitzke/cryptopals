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

#pragma once

#include <unordered_map>

namespace cryptopals {
template <typename T>
class Counter {
 public:
  size_t add(const T &val) {
    auto it = counts_.find(val);
    if (it == counts_.end()) {
      counts_.emplace(val, 1);
      return 1;
    }
    it->second++;
    return it->second;
  }

  std::pair<T, size_t> max_count() const {
    auto best = counts_.begin();
    for (auto it = counts_.begin(); it != counts_.end(); it++) {
      if (it->second > best->second) {
        best = it;
      }
    }
    return *best;
  }

 private:
  std::unordered_map<T, size_t> counts_;
};
}  // namespace cryptopals
