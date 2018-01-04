#pragma once

#include <cstddef>
#include <string>

// Decode a hexstring into a buffer.
void decode_hex(const std::string &hexstring, uint8_t *buf, size_t sz);

std::string encode_hex(const uint8_t *buf, size_t sz);

std::string to_base64(const uint8_t *buf, size_t sz);

// a, b, and c should all be buffers of size sz. Then a and b are xored to
// produce c.
void xor_buffers(uint8_t *a, uint8_t *b, uint8_t *c, size_t sz);
