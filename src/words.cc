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

#include "./words.h"

#include <ctype.h>
#include <cassert>
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <unordered_map>
#include <unordered_set>

namespace cryptopals {

static std::unordered_set<std::string> words_;

inline std::string lowercase(const std::string &input) {
  std::ostringstream os;
  for (char c : input) {
    os << tolower(c);
  }
  return os.str();
}

template <typename T>
static float distance(const std::unordered_map<T, float> &ref_frequency,
                      const std::unordered_map<T, size_t> &appearances) {
  assert(ref_frequency.size() == appearances.size());

  float count = 0;
  for (const auto &kv : appearances) {
    count += kv.second;
  }
  if (!count) return INFINITY;

  float dist = 0;
  for (const auto &pr : appearances) {
    float actual_frequency = (float)pr.second / (float)count;
    const auto it = ref_frequency.find(pr.first);
    assert(it != ref_frequency.end());
    float d = actual_frequency - it->second;
    dist += d * d;
  }
  return dist;
}

float score_text(const std::string &text, bool use_dict) {
  // copied from https://en.wikipedia.org/wiki/Letter_frequency
  static const std::unordered_map<char, float> char_frequencies{
      {'a', 8.167e-2},  {'b', 1.492e-2}, {'c', 2.782e-2}, {'d', 4.253e-2},
      {'e', 12.702e-2}, {'f', 2.228e-2}, {'g', 2.015e-2}, {'h', 6.094e-2},
      {'i', 6.966e-2},  {'j', 0.153e-2}, {'k', 0.772e-2}, {'l', 4.025e-2},
      {'m', 2.406e-2},  {'n', 6.749e-2}, {'o', 7.507e-2}, {'p', 1.929e-2},
      {'q', 0.095e-2},  {'r', 5.987e-2}, {'s', 6.327e-2}, {'t', 9.056e-2},
      {'u', 2.758e-2},  {'v', 0.978e-2}, {'w', 2.360e-2}, {'x', 0.150e-2},
      {'y', 1.974e-2},  {'z', 0.074e-2}};

  const size_t length_overflow = 21;
  static const std::unordered_map<size_t, float> word_lengths{
      {1, 2.998e-2},  {2, 17.651e-2}, {3, 20.511e-2}, {4, 14.787e-2},
      {5, 10.700e-2}, {6, 8.388e-2},  {7, 7.939e-2},  {8, 5.943e-2},
      {9, 4.437e-2},  {10, 3.076e-2}, {11, 1.761e-2}, {12, 0.958e-2},
      {13, 0.518e-2}, {14, 0.222e-2}, {15, 0.076e-2}, {16, 0.020e-2},
      {17, 0.010e-2}, {18, 0.004e-2}, {19, 0.001e-2}, {20, 0.001e-2},
      {21, 0.000e-2}};

  // Build a map of char to count.
  std::unordered_map<char, size_t> char_counts = {
      {'a', 0}, {'b', 0}, {'c', 0}, {'d', 0}, {'e', 0}, {'f', 0}, {'g', 0},
      {'h', 0}, {'i', 0}, {'j', 0}, {'k', 0}, {'l', 0}, {'m', 0}, {'n', 0},
      {'o', 0}, {'p', 0}, {'q', 0}, {'r', 0}, {'s', 0}, {'t', 0}, {'u', 0},
      {'v', 0}, {'w', 0}, {'x', 0}, {'y', 0}, {'z', 0}};

  // Map of word frequencies
  std::unordered_map<size_t, size_t> word_counts = {
      {1, 0},  {2, 0},  {3, 0},  {4, 0},  {5, 0},  {6, 0},  {7, 0},
      {8, 0},  {9, 0},  {10, 0}, {11, 0}, {12, 0}, {13, 0}, {14, 0},
      {15, 0}, {16, 0}, {17, 0}, {18, 0}, {19, 0}, {20, 0}, {21, 0}};

  if (use_dict && words_.empty()) {
    std::ifstream infile("/usr/share/dict/words");
    std::string line;
    while (std::getline(infile, line)) {
      words_.insert(lowercase(line));
    }
  }

  float scale = 1;
  size_t dict_count = 0;
  std::ostringstream os;
  for (char c : text) {
    if (!isprint(c) && !isspace(c)) {
      scale *= 2;
      continue;
    }

    // check word length
    if (isspace(c)) {
      std::string s = os.str();
      if (s.size()) {
        std::size_t word_size = std::min(s.size(), length_overflow);
        word_counts[word_size]++;
        if (use_dict && words_.find(lowercase(s)) != words_.end()) {
          dict_count++;
        }
        std::ostringstream empty;
        os.swap(empty);
      }
      continue;
    }

    // skip non-alpha
    if (!isalpha(c)) continue;

    // convert to downcase
    if (isupper(c)) {
      c = tolower(c);
    }
    char_counts[c]++;
    os << c;
  }

  const float char_dist = distance(char_frequencies, char_counts);
  const float word_dist = distance(word_lengths, word_counts);
  float rt = char_dist * word_dist * scale;
  if (use_dict && dict_count) {
    rt /= dict_count;
  }
  return rt;
}

}  // namespace cryptopals
