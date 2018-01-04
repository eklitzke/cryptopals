#pragma once

#include "./problem.h"
#include "./util.h"

#include <cassert>
#include <cmath>
#include <iostream>
#include <string>
#include <unordered_map>

#define DECLARE(x, y) \
  virtual std::pair<int, int> number() const { return {x, y}; }

namespace cryptopals {
struct Problem_1_1 : Problem {
 public:
  DECLARE(1, 1)

  virtual bool test() {
    const std::string inp =
        "49276d206b696c6c696e6720796f757220627261696e206c696b65206120706f6973"
        "6f6e6f7573206d757368726f6f6d";
    uint8_t buf[48];
    decode_hex(inp, buf, sizeof buf);
    return to_base64(buf, sizeof buf) ==
           "SSdtIGtpbGxpbmcgeW91ciBicmFpbiBsaWtlIGEgcG9pc29ub3VzIG11c2hyb29t";
  }
};

struct Problem_1_2 : Problem {
 public:
  DECLARE(1, 2)

  virtual bool test() {
    const std::string as = "1c0111001f010100061a024b53535009181c";
    const std::string bs = "686974207468652062756c6c277320657965";
    uint8_t a[18], b[18], c[18];
    decode_hex(as, a, sizeof a);
    decode_hex(bs, b, sizeof b);
    xor_buffers(a, b, c, 18);
    return encode_hex(c, sizeof c) == "746865206b696420646f6e277420706c6179";
  }
};

struct Problem_1_3 : Problem {
 public:
  DECLARE(1, 3)

  // Get the score for a string; lower is better.
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
      delta += fabsf(actual_frequency - it->second);
    }
    return delta;
  }

  virtual bool test() {
    const std::string test =
        "1b37373331363f78151b7f2b783431333d78397828372d363c78373e783a393b3736";
    uint8_t buf[34];

    std::string best_string;
    float best_score = 1000;
    uint8_t best_key = 0;

    for (int key = 0; key <= 255; key++) {
      decode_hex(test, buf, sizeof buf);
      const uint8_t k = static_cast<uint8_t>(key);
      for (size_t i = 0; i < sizeof buf; i++) {
        buf[i] ^= k;
      }
      std::string s{reinterpret_cast<const char *>(buf), sizeof buf};
      float score = score_string(s);
      if (score < best_score) {
        best_score = score;
        best_key = k;
        best_string = s;
      }
    }
    return best_string == "Cooking MC's like a pound of bacon";
  }
};

void add_solutions(ProblemManager *manager);
}  // namespace cryptopals
