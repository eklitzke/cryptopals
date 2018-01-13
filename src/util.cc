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

#include "./util.h"

#include <fstream>
#include <memory>

#include "./aes.hpp"

namespace cryptopals {
static std::random_device rd;
static std::mt19937_64 rg(rd());

bool rand_bool(void) {
  static std::uniform_int_distribution<uint64_t> binary_dist(0, 1);
  return binary_dist(rg);
}

std::string rand_string(size_t nbytes) {
  std::ifstream urand("/dev/urandom", std::ios::binary);
  std::unique_ptr<char[]> buf = std::unique_ptr<char[]>(new char[nbytes]);
  urand.read(buf.get(), nbytes);
  return {buf.get(), nbytes};
}

std::string rand_string(size_t min_bytes, size_t max_bytes) {
  std::uniform_int_distribution<size_t> dist(min_bytes, max_bytes);
  std::mt19937_64 rg(rd());
  return rand_string(dist(rg));
}

// return a random AES key
std::string rand_key(void) { return rand_string(AES_BLOCKLEN); }
}
