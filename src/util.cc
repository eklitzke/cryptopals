#include "./util.h"

#include <cassert>
#include <cstring>
#include <memory>
#include <sstream>

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
