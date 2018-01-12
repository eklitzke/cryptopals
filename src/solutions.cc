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

#include <cassert>
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <unordered_map>

#include "./buffer.h"
#include "./solutions.h"

namespace cryptopals {
void add_all_solutions(ProblemManager *manager) {
  manager->AddSolution(1, 1, []() {
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

  manager->AddSolution(1, 2, []() {
    Buffer a("1c0111001f010100061a024b53535009181c", HEX);
    Buffer b("686974207468652062756c6c277320657965", HEX);
    a ^= b;
    return a.encode_hex() == "746865206b696420646f6e277420706c6179";
  });

  manager->AddSolution(1, 3, []() {
    Buffer buf(
        "1b37373331363f78151b7f2b783431333d78397828372d363c78373e783a393b3736",
        HEX);
    std::string best_string;
    buf.guess_single_byte_xor_key(&best_string);
    return best_string == "Cooking MC's like a pound of bacon";
  });

  manager->AddSolution(1, 4, []() {
    std::string best_string;
    float best_score = INFINITY;

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

  manager->AddSolution(1, 5, []() {
    Buffer buf(
        "Burning 'em, if you ain't quick and nimble\nI go crazy when I hear "
        "a cymbal");
    buf.xor_string("ICE");
    return buf.encode_hex() ==
           "0b3637272a2b2e63622c2e69692a23693a2a3c6324202d623d63343c2a262263242"
           "72765272a282b2f20430a652e2c652a3124333a653e2b2027630c692b2028316528"
           "6326302e27282f";
  });

  manager->AddSolution(1, 6, []() {
    Buffer a("this is a test");
    Buffer b("wokka wokka!!!");
    if (a.edit_distance(b) != 37) {
      return false;
    }
    Buffer buf("data/6.txt", BASE64_FILE);
    std::string key = buf.guess_vigenere_key(2, 40);
    return key == "Terminator X: Bring the noise";
  });

  manager->AddSolution(1, 7, []() {
    Buffer buf("data/7.txt", BASE64_FILE);
    buf.aes_ecb_decrypt("YELLOW SUBMARINE");
    return buf.encode().find("Play that funky music") != std::string::npos;
  });

  manager->AddSolution(1, 8, []() {
    size_t best = 0;
    std::string best_line = "";

    // find the line with the most repeated chunks of 4 bytes
    std::ifstream infile("data/8.txt");
    std::string line;
    while (std::getline(infile, line)) {
      Buffer buf(line, HEX);
      assert(buf.size() % 4 == 0);
      std::unordered_map<std::string, size_t> counts;
      for (size_t i = 0; i < buf.size(); i += 4) {
        std::string s = buf.slice(i, i + 4).encode();
        auto it = counts.find(s);
        if (it == counts.end()) {
          counts.emplace(s, 1);
        } else {
          counts[s]++;
          if (counts[s] > best) {
            best = counts[s];
            best_line = line;
          }
        }
      }
    }
    return best_line ==
           "d880619740a8a19b7840a8a31c810a3d08649af70dc06f4fd5d2d69c744cd283e2d"
           "d052f6b641dbf9d11b0348542bb5708649af70dc06f4fd5d2d69c744cd2839475c9"
           "dfdbc1d46597949d9c7e82bf5a08649af70dc06f4fd5d2d69c744cd28397a93eab8"
           "d6aecd566489154789a6b0308649af70dc06f4fd5d2d69c744cd283d403180c98c8"
           "f6db1f2a3f9c4040deb0ab51b29933f2c123c58386b06fba186a";
  });

  manager->AddSolution(2, 9, []() {
    Buffer yellow("YELLOW SUBMARINE");
    yellow.pad_pkcs7(20);
    if (yellow.encode() != "YELLOW SUBMARINE\x04\x04\x04\x04") {
      return false;
    }
    yellow.unpad_pkcs7();
    if (yellow.encode() != "YELLOW SUBMARINE") {
      return false;
    }
    yellow.pad_pkcs7(16);
    if (yellow.encode() !=
        "YELLOW "
        "SUBMARINE\x10\x10\x10\x10\x10\x10\x10\x10\x10\x10\x10\x10\x10\x10\x10"
        "\x10") {
      return false;
    }
    yellow.unpad_pkcs7();
    return yellow.encode() == "YELLOW SUBMARINE";
  });

  manager->AddSolution(2, 10, []() {
    Buffer buf("data/10.txt", BASE64_FILE);
    buf.aes_cbc_decrypt("YELLOW SUBMARINE");
    return buf.encode().find("Play that funky music") != std::string::npos;
  });
}
}  // namespace cryptopals
