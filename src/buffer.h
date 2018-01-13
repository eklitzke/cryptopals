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
  BASE64_FILE,
};

class Buffer {
 public:
  Buffer() {}
  explicit Buffer(const std::string &s, Encoding encoding = STRING);
  explicit Buffer(const std::vector<uint8_t> &buf) : buf_(buf) {}
  Buffer(const Buffer &other) : buf_(other.buf_) {}

  inline size_t size() const { return buf_.size(); }

  inline bool operator==(const Buffer &other) const {
    return buf_ == other.buf_;
  }

  inline void append(const Buffer &other) {
    buf_.insert(buf_.end(), other.buf_.begin(), other.buf_.end());
  }

  inline void append(uint8_t byte) { buf_.push_back(byte); }

  Buffer slice(size_t start, size_t end) const;

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

  // add padding bytes, as defined by pkcs #7
  void pad_pkcs7(uint8_t target_multiple);

  // undo padding bytes, as defined by pkcs #7
  void unpad_pkcs7();

  // ecb decrypt *in place*
  void aes_ecb_decrypt(const std::string &key, bool pkcs7 = true);

  // cbc decrypt *in place*
  void aes_cbc_decrypt(const std::string &key, bool pkcs7 = true);

  // ecb encrypt *in place*
  void aes_ecb_encrypt(const std::string &key, bool pkcs7 = true);

  // cbc encrypt *in place*
  void aes_cbc_encrypt(const std::string &key, bool pkcs7 = true);

  // add [min_bytes, max_bytes] random data at the head of the string, and same
  // at the end
  void obfuscate(size_t min_bytes, size_t max_bytes);

  // return ECB or CBC based on our guess
  std::string guess_encryption_mode(size_t min_key_size = 8,
                                    size_t max_key_size = 32) const;
  std::string guess_encryption_mode(size_t key_size) const {
    return guess_encryption_mode(key_size, key_size);
  }

 private:
  std::vector<uint8_t> buf_;

  // set the buffer contents based on base64 data
  void set_base64_data(const std::string &s);

  // guess the key, and return it
  std::string guess_vigenere_key(size_t key_length, float *score) const;

  // try a single byte xor key, and return the score for the key
  float try_single_byte_xor_key(uint8_t key) const;

  // vertically stack the buffers along some width
  std::vector<Buffer> stack_and_transpose(size_t width) const;
};
}  // namespace cryptopals
