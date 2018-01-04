#include "./util.h"

#include <cassert>
#include <cstring>
#include <unordered_map>

namespace cryptopals {
float score_string(const std::string &s) {
  // copied from https://en.wikipedia.org/wiki/Letter_frequency
  static const std::unordered_map<char, float> frequencies{
      {'a', 8.167e-2},  {'b', 1.492e-2}, {'c', 2.782e-2}, {'d', 4.253e-2},
      {'e', 12.702e-2}, {'f', 2.228e-2}, {'g', 2.015e-2}, {'h', 6.094e-2},
      {'i', 6.966e-2},  {'j', 0.153e-2}, {'k', 0.772e-2}, {'l', 4.025e-2},
      {'m', 2.406e-2},  {'n', 6.749e-2}, {'o', 7.507e-2}, {'p', 1.929e-2},
      {'q', 0.095e-2},  {'r', 5.987e-2}, {'s', 6.327e-2}, {'t', 9.056e-2},
      {'u', 2.758e-2},  {'v', 0.978e-2}, {'w', 2.360e-2}, {'x', 0.150e-2},
      {'y', 1.974e-2},  {'z', 0.074e-2}};
  assert(frequencies.size() == 26);

  // Build a map of char to count.
  size_t chars_total = 0;
  std::unordered_map<char, size_t> counts = {
      {'a', 0}, {'b', 0}, {'c', 0}, {'d', 0}, {'e', 0}, {'f', 0}, {'g', 0},
      {'h', 0}, {'i', 0}, {'j', 0}, {'k', 0}, {'l', 0}, {'m', 0}, {'n', 0},
      {'o', 0}, {'p', 0}, {'q', 0}, {'r', 0}, {'s', 0}, {'t', 0}, {'u', 0},
      {'v', 0}, {'w', 0}, {'x', 0}, {'y', 0}, {'z', 0}};
  assert(counts.size() == 26);
  for (char c : s) {
    // skip non-alpha
    if (!isalpha(c)) continue;

    // convert to downcase
    if (isupper(c)) {
      c = c - 'A' + 'a';
    }
    counts[c]++;
    chars_total++;
  }
  assert(counts.size() == 26);

  float delta = 0;
  for (const auto &pr : counts) {
    float actual_frequency = (float)pr.second / (float)chars_total;
    const auto it = frequencies.find(pr.first);
    assert(it != frequencies.end());
    float d = actual_frequency - it->second;
    delta += d * d;
  }

  // penalize bad characters
  for (char c : s) {
    if (isspace(c) || isalpha(c)) {
      continue;
    }
    if (!isprint(c)) {
      delta += 100;
    } else {
      delta += 10;
    }
  }

  // penalize if does not start with upper
  if (!isupper(*s.begin())) {
    delta += 1;
  }

  return delta;
}
}  // namespace cryptopals
