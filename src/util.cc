#include "./util.h"

#include <cassert>
#include <cmath>
#include <cstring>
#include <memory>
#include <sstream>
#include <unordered_map>

namespace cryptopals {
// convert a 3 byte string to a 4 byte base64
inline void b64_helper(std::ostringstream &os, uint8_t *inp) {
  static const char lut[] =
      "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
  const uint8_t b1 = inp[0] >> 2;
  assert(b1 < 64);
  const uint8_t b2 = ((inp[0] & 0x3) << 4) + (inp[1] >> 4);
  assert(b2 < 64);
  const uint8_t b3 = ((inp[1] & 0xf) << 2) + (inp[2] >> 6);
  assert(b3 < 64);
  const uint8_t b4 = inp[2] & 0x3f;
  assert(b4 < 64);
  os << lut[b1] << lut[b2] << lut[b3] << lut[b4];
}

// another helper
inline uint8_t hex_to_bin(const char *str) {
  assert(isxdigit(str[0]) && isxdigit(str[1]));
  uint8_t result = (isdigit(str[0]) ? str[0] - '0' : 10 + str[0] - 'a') << 4;
  result += isdigit(str[1]) ? str[1] - '0' : 10 + str[1] - 'a';
  return result;
}

// encode a value in the range 0-f
inline char bin_to_hex(uint8_t val) {
  assert(val <= 0xf);
  if (val < 0xa) {
    return '0' + val;
  }
  return 'a' + val - 0xa;
}

void decode_hex(const std::string &hexstring, uint8_t *buf, size_t sz) {
  assert(hexstring.size() % 2 == 0);
  assert(sz >= hexstring.size() / 2);
  for (size_t i = 0; i < hexstring.size() / 2; i++) {
    *(buf + i) = hex_to_bin(hexstring.data() + i * 2);
  }
}

std::string encode_hex(const uint8_t *buf, size_t sz) {
  std::ostringstream os;
  for (size_t i = 0; i < sz; i++) {
    os << bin_to_hex((buf[i] & 0xf0) >> 4) << bin_to_hex(buf[i] & 0xf);
  }
  return os.str();
}

std::string to_base64(const uint8_t *buf, size_t sz) {
  // Kind of annoying, we need to pad the buffer if sz is not divisible by
  // three.
  size_t padding = sz % 3;
  if (padding) {
    padding = 3 - padding;
  }

  sz += padding;
  std::unique_ptr<uint8_t[]> bytes(new uint8_t[sz]);
  std::memset(bytes.get(), 0, sz);
  std::memmove(bytes.get(), buf, sz);

  std::ostringstream os;
  for (size_t i = 0; i < sz; i += 3) {
    b64_helper(os, bytes.get() + i);
  }
  std::string ret = os.str();
  assert(ret.size() % 4 == 0);
  for (size_t i = 0; i < padding; i++) {
    ret[ret.size() - 1 - i] = '=';
  }
  return ret;
}

void xor_buffers(uint8_t *a, uint8_t *b, uint8_t *c, size_t sz) {
  for (size_t i = 0; i < sz; i++) {
    c[i] = a[i] ^ b[i];
  }
}

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

  return delta;
}
}  // namespace cryptopals
