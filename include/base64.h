// Copyright 2025 c-w-Inf. All Rights Reserved.

#ifndef MYRSA_INCLUDE_BASE64_H_
#define MYRSA_INCLUDE_BASE64_H_

#include <stddef.h>
#include <stdint.h>

size_t Base64_encode_size (size_t size);
size_t Base64_decode_size (size_t len);

void Base64_encode (const uint8_t* data, size_t size, char* code);
size_t Base64_decode (const char* code, uint8_t* data);

#endif  // MYRSA_INCLUDE_BASE64_H_
