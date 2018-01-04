#pragma once

#include <cstddef>
#include <memory>
#include <string>
#include <vector>

namespace cryptopals {

class Buffer {
 public:
  Buffer() {}
  explicit Buffer(const std::string &s, bool is_hex = true);
  Buffer(const Buffer &other) : buf_(other.buf_) {}

  std::string encode_raw() const;
  std::string encode_hex() const;
  std::string encode_base64() const;

  void operator^=(const Buffer &other);
  inline uint8_t operator[](size_t i) { return buf_[i]; }

  void xor_byte(uint8_t key);

  void xor_string(const std::string &key);

  inline size_t size() const { return buf_.size(); }

 private:
  std::vector<uint8_t> buf_;
};
}  // namespace cryptopals
