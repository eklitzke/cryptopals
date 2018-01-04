#include <stdlib.h>

#include <cassert>
#include <iostream>
#include <string>

#include "./base64.h"

int main(int argc, char **argv) {
  if (argc != 3) {
    std::cout << "Usage: " << argv[0] << " <set> <challenge>\n";
    return 1;
  }
  errno = 0;
  unsigned long int num = strtoul(argv[1], nullptr, 10) * 100;
  assert(errno == 0);
  num += strtoul(argv[2], nullptr, 10);
  assert(errno == 0);

  switch (num) {
    case 101: {
      const std::string inp =
          "49276d206b696c6c696e6720796f757220627261696e206c696b65206120706f6973"
          "6f6e6f7573206d757368726f6f6d";
      const std::string outp =
          "SSdtIGtpbGxpbmcgeW91ciBicmFpbiBsaWtlIGEgcG9pc29ub3VzIG11c2hyb29t";
      assert(hex_to_base64(inp) == outp);
      break;
    }
    case 102: {
      const std::string as = "1c0111001f010100061a024b53535009181c";
      const std::string bs = "686974207468652062756c6c277320657965";
      uint8_t a[18], b[18], c[18];
      decode_hex(as, a, sizeof a);
      decode_hex(bs, b, sizeof b);
      xor_buffers(a, b, c, 18);
      assert(encode_hex(c, sizeof c) == "746865206b696420646f6e277420706c6179");
      break;
    }
    default:
      std::cerr << "Unknown problem " << num << "\n";
      return 1;
  }
  return 0;
}
