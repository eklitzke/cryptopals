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

#pragma once

#include <cstddef>
#include <iterator>
#include <memory>
#include <string>
#include <vector>

namespace cryptopals {

enum Encoding {
  STRING,
  HEX,
  BASE64,
};

class Buffer {
 private:
  Buffer(const std::vector<uint8_t> &buf) : buf_(buf) {}

 public:
  Buffer() {}
  explicit Buffer(const std::string &s, Encoding encoding = STRING);
  Buffer(const Buffer &other) : buf_(other.buf_) {}

  Buffer slice(size_t start, size_t end) const;
  Buffer copy() const;

  inline std::string encode() const {
    return {reinterpret_cast<const char *>(buf_.data()), buf_.size()};
  }
  std::string encode_hex() const;
  std::string encode_base64() const;

  // Get the score of this buffer as a string.
  float string_score() const;

  void operator^=(const Buffer &other);

  inline uint8_t operator[](size_t i) const { return buf_[i]; }

  void xor_byte(uint8_t key);

  void xor_string(const std::string &key);

  uint8_t guess_single_byte_xor_key(std::string *out = nullptr,
                                    float *score = nullptr) const;

  // number of bits in the delta between the two
  size_t edit_distance(const Buffer &other);

  // guess the key for a vigenere cipher
  std::string guess_vigenere_key(size_t min_key_size, size_t max_key_size,
                                 size_t guesses = 5) const;

  inline size_t size() const { return buf_.size(); }

  // vertically stack the buffers along some width
  std::vector<Buffer> stack_and_transpose(size_t width) const;

 private:
  std::vector<uint8_t> buf_;

  // guess the key, and return it
  std::string guess_vigenere_key(size_t key_length, float *score) const;

  // try a single byte xor key, and return the score for the key
  float try_single_byte_xor_key(uint8_t key) const;
#if 0
  inline float try_single_byte_xor_key(int key) const {
    return try_single_byte_xor_key(static_cast<uint8_t>(key));
  }
#endif

  std::vector<uint8_t> guess_single_byte_xor_keys(size_t count) const;
};
}  // namespace cryptopals
