// Copyright 2025 c-w-Inf. All Rights Reserved.

#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace rsa::base64 {

std::string encode (const std::vector<uint8_t>& data);
std::vector<uint8_t> decode (const std::string& code);

}  // namespace rsa::base64
