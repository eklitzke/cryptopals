#include "./buffer.h"

#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstring>
#include <iostream>
#include <limits>
#include <sstream>
#include <unordered_map>

#include "./words.h"

namespace cryptopals {

Buffer Buffer::slice(size_t start, size_t end) const {
  end = std::min(end, buf_.size());
  return Buffer(
      std::vector<uint8_t>(buf_.cbegin() + start, buf_.cbegin() + end));
}

Buffer Buffer::copy() const { return Buffer(*this); }

static const std::string b64_lut =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

// convert a 3 byte string to a 4 byte base64
inline void bin_to_b64(std::ostringstream &os, uint8_t *inp) {
  const uint8_t b1 = inp[0] >> 2;
  assert(b1 < 64);
  const uint8_t b2 = ((inp[0] & 0x3) << 4) + (inp[1] >> 4);
  assert(b2 < 64);
  const uint8_t b3 = ((inp[1] & 0xf) << 2) + (inp[2] >> 6);
  assert(b3 < 64);
  const uint8_t b4 = inp[2] & 0x3f;
  assert(b4 < 64);
  os << b64_lut[b1] << b64_lut[b2] << b64_lut[b3] << b64_lut[b4];
}

inline uint8_t b64_char_to_num(char c) {
  size_t pos = b64_lut.find(c);
  if (pos == std::string::npos) {
    return 0;
  }
  assert(pos < 64);
  return static_cast<uint8_t>(pos);
}

inline void b64_to_bin(std::vector<uint8_t> &out, const char *data) {
  size_t padding = 0;
  for (int i = 0; i < 4; i++) {
    if (data[i] == '=') {
      padding++;
    }
  }
  assert(padding <= 2);

  uint8_t a = b64_char_to_num(data[0]);
  uint8_t b = b64_char_to_num(data[1]);

  out.push_back((a << 2) + ((b & 0x30) >> 4));
  if (padding == 2) {
    return;
  }
  uint8_t c = b64_char_to_num(data[2]);
  out.push_back(((b & 0xf) << 4) + (c >> 2));

  if (padding == 1) {
    return;
  }
  uint8_t d = b64_char_to_num(data[3]);
  out.push_back(((c & 0x3) << 6) + d);
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

Buffer::Buffer(const std::string &s, Encoding encoding) {
  switch (encoding) {
    case STRING:
      for (char c : s) {
        buf_.push_back(static_cast<uint8_t>(c));
      }
      break;
    case HEX:
      assert(s.size() % 2 == 0);
      for (size_t i = 0; i < s.size() / 2; i++) {
        buf_.push_back(hex_to_bin(s.data() + i * 2));
      }
      break;
    case BASE64: {
      assert(s.size() % 4 == 0);
      for (size_t i = 0; i < s.size() / 4; i++) {
        b64_to_bin(buf_, s.data() + i * 4);
      }
      break;
    }
    default:
      assert(false);  // not reached
      break;
  }
}

std::string Buffer::encode_hex() const {
  std::ostringstream os;
  for (uint8_t c : buf_) {
    os << bin_to_hex((c & 0xf0) >> 4) << bin_to_hex(c & 0xf);
  }
  return os.str();
}

std::string Buffer::encode_base64() const {
  // Kind of annoying, we need to pad the buffer if sz is not divisible by
  // three.
  size_t sz = buf_.size();
  size_t padding = sz % 3;
  if (padding) {
    padding = 3 - padding;
  }
  sz += padding;
  std::unique_ptr<uint8_t[]> bytes(new uint8_t[sz]);
  std::memset(bytes.get(), 0, sz);
  std::memmove(bytes.get(), buf_.data(), sz);

  std::ostringstream os;
  for (size_t i = 0; i < sz; i += 3) {
    bin_to_b64(os, bytes.get() + i);
  }
  std::string ret = os.str();
  assert(ret.size() % 4 == 0);
  for (size_t i = 0; i < padding; i++) {
    ret[ret.size() - 1 - i] = '=';
  }
  return ret;
}

float Buffer::string_score() const { return score_text(encode()); }

void Buffer::xor_byte(uint8_t k) {
  for (size_t i = 0; i < buf_.size(); i++) {
    buf_[i] ^= k;
  }
}

void Buffer::xor_string(const std::string &key) {
  assert(key.size());
  for (size_t i = 0; i < buf_.size(); i++) {
    buf_[i] ^= static_cast<uint8_t>(key[i % key.size()]);
  }
};

float Buffer::try_single_byte_xor_key(uint8_t key) const {
  Buffer clone = copy();
  clone.xor_byte(key);
  return clone.string_score();
}

std::vector<uint8_t> Buffer::guess_single_byte_xor_keys(size_t count) const {
  std::vector<std::pair<uint8_t, float> > scores;
  for (int key = 0; key <= 255; key++) {
    scores.emplace_back(key, try_single_byte_xor_key(key));
  }
  std::sort(scores.begin(), scores.end(), [](const auto &lhs, const auto &rhs) {
    return lhs.second < rhs.second;
  });
  count = std::min(count, scores.size());

  std::vector<uint8_t> out;
  for (size_t i = 0; i < count; i++) {
    out.push_back(scores[i].first);
  }
  return out;
}

uint8_t Buffer::guess_single_byte_xor_key(std::string *out,
                                          float *score) const {
  uint8_t best_key = 0;
  float best_score = std::numeric_limits<float>::max();
  for (int key = 0; key <= 255; key++) {
    uint8_t k = static_cast<uint8_t>(key);
    auto clone = copy();
    clone.xor_byte(k);
    float val = clone.string_score();
    if (val < best_score) {
      best_score = val;
      best_key = k;
      if (out != nullptr) *out = clone.encode();
      if (score != nullptr) *score = val;
    }
  }
  return best_key;
}

size_t Buffer::edit_distance(const Buffer &other) {
  assert(size() == other.size());
  size_t distance = 0;
  for (size_t i = 0; i < buf_.size(); i++) {
    const uint8_t diff = buf_[i] ^ other.buf_[i];
    distance += __builtin_popcount(diff);
  }
  return distance;
}

void Buffer::operator^=(const Buffer &other) {
  assert(size() == other.size());
  for (size_t i = 0; i < buf_.size(); i++) {
    buf_[i] ^= other.buf_[i];
  }
}

std::string Buffer::guess_vigenere_key(size_t min_key_size, size_t max_key_size,
                                       size_t guesses) const {
  // First we need to guess the keysize. We look at the "entropy" from comparing
  // the first few word sized guesses.
  std::vector<std::pair<size_t, float> > key_size_entropies;
  for (size_t width = min_key_size; width < max_key_size; width++) {
    float dist = 0;
    for (int j = 0; j < 4; j++) {
      size_t off = width * j;
      Buffer first = slice(off, off + width);
      Buffer second = slice(off + width, off + 2 * width);
      dist += first.edit_distance(second);
    }
    key_size_entropies.push_back({width, dist / (float)width});
  }

  // Sort from low to high, by normalized edit distance.
  std::sort(
      key_size_entropies.begin(), key_size_entropies.end(),
      [](const auto &lhs, const auto &rhs) { return lhs.second < rhs.second; });

  // Try the top guesses.
  std::string best_string, best_key;
  float best_score = std::numeric_limits<float>::max();
  for (size_t i = 0; i < std::min(guesses, key_size_entropies.size()); i++) {
    size_t key_size = key_size_entropies[i].first;
    float score;
    std::string key = guess_vigenere_key(key_size, &score);
    if (score < best_score) {
      best_score = score;
      best_key = key;
      Buffer clone = copy();
      clone.xor_string(key);
      best_string = clone.encode();
#if 0
      std::cout << "score: " << best_score << ", key_size: " << key_size
                << ", key : " << key << ", string = " << best_string
                << std::endl;
#endif
    }
  }

  return best_key;
}

std::string Buffer::guess_vigenere_key(size_t key_length, float *score) const {
  std::ostringstream os;
  for (const auto &buf : stack_and_transpose(key_length)) {
    os << buf.guess_single_byte_xor_key();
  }

  const std::string key = os.str();
  assert(key.size() == key_length);
  if (score != nullptr) {
    auto clone = copy();
    clone.xor_string(key);
    *score = clone.string_score();
  }
  return key;
}

std::vector<Buffer> Buffer::stack_and_transpose(size_t width) const {
  // First stack them.
  std::vector<Buffer> vstack;
  for (size_t i = 0; i < buf_.size(); i += width) {
    vstack.push_back(slice(i, i + width));
  }

  std::vector<Buffer> transpose;
  for (size_t i = 0; i < width; i++) {
    std::ostringstream os;
    for (const auto &buf : vstack) {
      if (i < buf.size()) {
        os << buf[i];
      }
    }
    transpose.emplace_back(os.str(), STRING);
  }
  return transpose;
}
}  // namespace cryptopals
