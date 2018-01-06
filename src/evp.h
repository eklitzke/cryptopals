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

#include <cassert>
#include <string>

#include <openssl/evp.h>

namespace cryptopals {
class CipherCtx {
 public:
  CipherCtx() : ctx_(EVP_CIPHER_CTX_new()) { assert(ctx_ != nullptr); }
  ~CipherCtx() { EVP_CIPHER_CTX_free(ctx_); }

  std::string decrypt_aes_128_ecb(const std::string &ciphertext,
                                  const std::string &key);

 private:
  EVP_CIPHER_CTX *ctx_;
};

}  // namespace cryptopals
