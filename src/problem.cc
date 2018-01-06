#include "./problem.h"

#include <cassert>
#include <fstream>
#include <iostream>
#include <limits>
#include <sstream>
#include <utility>

#include "./buffer.h"

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

ProblemManager::ProblemManager() {
  AddSolution(1, 1, []() {
    Buffer a(
        "49276d206b696c6c696e6720796f757220627261696e206c696b65206120706f69736f"
        "6e6f7573206d757368726f6f6d",
        HEX);
    if (a.encode_base64() !=
        "SSdtIGtpbGxpbmcgeW91ciBicmFpbiBsaWtlIGEgcG9pc29ub3VzIG11c2hyb29t") {
      return false;
    }

    Buffer b("SSdtIGtpbGxpbmcgeW91ciBicmFpbiBsaWtlIGEgcG9pc29ub3VzIG11c2hyb29t",
             BASE64);
    return b.encode_hex() ==
           "49276d206b696c6c696e6720796f757220627261696e206c696b65206120706f697"
           "36f6e6f7573206d757368726f6f6d";
  });

  AddSolution(1, 2, []() {
    Buffer a("1c0111001f010100061a024b53535009181c", HEX);
    Buffer b("686974207468652062756c6c277320657965", HEX);
    a ^= b;
    return a.encode_hex() == "746865206b696420646f6e277420706c6179";
  });

  AddSolution(1, 3, []() {
    Buffer buf(
        "1b37373331363f78151b7f2b783431333d78397828372d363c78373e783a393b3736",
        HEX);
    std::string best_string;
    buf.guess_single_byte_xor_key(&best_string);
    return best_string == "Cooking MC's like a pound of bacon";
  });

  AddSolution(1, 4, []() {
    std::string best_string;
    float best_score = std::numeric_limits<float>::max();

    std::ifstream infile("data/4.txt");
    std::string line;
    while (std::getline(infile, line)) {
      assert(line.size() <= 60);
      Buffer buf(line, HEX);
      std::string s;
      float score;
      buf.guess_single_byte_xor_key(&s, &score);
      if (score < best_score) {
        best_string = s;
        best_score = score;
      }
    }
    return best_string == "Now that the party is jumping\n";
  });

  AddSolution(1, 5, []() {
    Buffer buf(
        "Burning 'em, if you ain't quick and nimble\nI go crazy when I hear "
        "a cymbal");
    buf.xor_string("ICE");
    return buf.encode_hex() ==
           "0b3637272a2b2e63622c2e69692a23693a2a3c6324202d623d63343c2a262263242"
           "72765272a282b2f20430a652e2c652a3124333a653e2b2027630c692b2028316528"
           "6326302e27282f";
  });

  AddSolution(1, 6, []() {
    Buffer a("this is a test");
    Buffer b("wokka wokka!!!");
    if (a.edit_distance(b) != 37) {
      return false;
    }

    std::ifstream infile("data/6.txt");
    std::string line;
    std::ostringstream os;
    while (std::getline(infile, line)) {
      os << line;
    };
    Buffer buf(os.str(), BASE64);

    std::string key = buf.guess_vigenere_key(2, 40);
    return key == "Terminator X: Bring the noise";
  });
}

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
