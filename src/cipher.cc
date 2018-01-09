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

#include "./cipher.h"

#include <cassert>
#include <cstring>
#include <memory>

#include "./aes.hpp"

namespace cryptopals {
std::string aes_ecb_decrypt(const std::string &ciphertext,
                            const std::string &key) {
  assert(ciphertext.size() % AES_BLOCKLEN == 0);
  AES_ctx ctx;
  AES_init_ctx(&ctx, (const uint8_t *)key.c_str());

  std::string copy = ciphertext;
  for (size_t i = 0; i < copy.size(); i += AES_BLOCKLEN) {
    AES_ECB_decrypt(&ctx, (uint8_t *)copy.data() + i);
  }
  return copy;
}
}  // namespace cryptopals
