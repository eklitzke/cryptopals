#pragma once

#include "./problem.h"
#include "./util.h"

#include <string>

namespace cryptopals {
struct Problem_1_1 : Problem {
 public:
  virtual std::pair<int, int> number() { return {1, 1}; }

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
  virtual std::pair<int, int> number() { return {1, 2}; }

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

void add_solutions(ProblemManager *manager);
}  // namespace cryptopals
