// Copyright 2025 c-w-Inf. All Rights Reserved.

#include "base64.hpp"

#include <stdint.h>

#include <sstream>

namespace rsa::base64 {

std::string encode (const std::vector<uint8_t>& data) {
    static constexpr char map[] =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz"
        "0123456789+/";

    std::stringstream ss;

    uint32_t buf;
    decltype (data.size ()) i = 0;
    while (i + 3 < data.size ()) {
        buf = data[i++];
        buf = (buf << 8) | data[i++];
        buf = (buf << 8) | data[i++];
        ss << map[buf >> 18];
        ss << map[(buf & 0x03f000) >> 12];
        ss << map[(buf & 0x000fc0) >> 6];
        ss << map[(buf & 0x00003f)];
    }

    if (i < data.size ()) {
        buf = data[i];
        if (i == data.size () - 1) {
            ss << map[buf >> 2];
            ss << map[(buf & 0x03) << 4];
            ss << '=';
            ss << '=';
        } else {
            buf = (buf << 8) | data[++i];
            ss << map[buf >> 10];
            ss << map[(buf & 0x03c0) >> 4];
            ss << map[(buf & 0x000f) << 2];
            ss << '=';
        }
    }

    return ss.str ();
}

std::vector<uint8_t> decode (const std::string& code) {
    static const uint8_t map[256] = {
        128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128,
        128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 62,
        128, 128, 128, 63,  52,  53,  54,  55,  56,  57,  58,  59,  60,  61,  128, 128, 128, 255, 128, 128, 128, 0,
        1,   2,   3,   4,   5,   6,   7,   8,   9,   10,  11,  12,  13,  14,  15,  16,  17,  18,  19,  20,  21,  22,
        23,  24,  25,  128, 128, 128, 128, 128, 128, 26,  27,  28,  29,  30,  31,  32,  33,  34,  35,  36,  37,  38,
        39,  40,  41,  42,  43,  44,  45,  46,  47,  48,  49,  50,  51,  128, 128, 128, 128, 128, 128, 128, 128, 128,
        128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128,
        128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128,
        128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128,
        128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128,
        128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128,
        128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128};

    std::vector<uint8_t> ret;

    uint32_t buf = 0;
    uint32_t phase = 18;
    for (char c : code) {
        uint8_t dat = map[static_cast<uint8_t> (c)];
        if (dat == 128)
            throw std::invalid_argument ("rsa::base64::decode");
        else if (dat == 255)
            break;

        buf |= dat << phase;
        if (phase) {
            phase -= 6;
        } else {
            ret.push_back (buf >> 16);
            ret.push_back ((buf & 0x00ff00) >> 8);
            ret.push_back (buf & 0x0000ff);

            buf = 0;
            phase = 18;
        }
    }

    if (phase == 6) {
        ret.push_back (buf >> 16);
    } else if (phase == 0) {
        ret.push_back (buf >> 16);
        ret.push_back ((buf & 0x00ff00) >> 8);
    }

    return ret;
}

}  // namespace rsa::base64
