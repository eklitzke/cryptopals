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

#include "./evp.h"

#include <memory>

namespace cryptopals {
std::string CipherCtx::decrypt_aes_128_ecb(const std::string &ciphertext,
                                           const std::string &key) {
  assert(EVP_DecryptInit_ex(ctx_, EVP_aes_128_ecb(), nullptr,
                            (const unsigned char *)key.c_str(), nullptr) == 1);
  auto plaintext = std::unique_ptr<uint8_t[]>(new uint8_t[ciphertext.size()]);
  int len;
  assert(EVP_DecryptUpdate(ctx_, plaintext.get(), &len,
                           (const unsigned char *)ciphertext.data(),
                           ciphertext.size()) == 1);
  size_t plaintext_len = len;
  assert(EVP_DecryptFinal_ex(ctx_, plaintext.get() + len, &len) == 1);
  plaintext_len += len;

  return {(const char *)plaintext.get(), plaintext_len};
}
}  // namespace cryptopals
