// Copyright 2025 c-w-Inf. All Rights Reserved.

#include "base64.h"

#include <stdint.h>

size_t Base64_encode_size (size_t size) { return (size + 2) / 3 * 4 + 1; }
size_t Base64_decode_size (size_t len) { return len / 4 * 3; }

void Base64_encode (const uint8_t* data, size_t size, char* code) {
    static const char map[] =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz"
        "0123456789+/";

    size_t s = 0;
    uint32_t buf;
    while (s + 3 < size) {
        buf = data[s++];
        buf = (buf << 8) | data[s++];
        buf = (buf << 8) | data[s++];
        *code++ = map[buf >> 18];
        *code++ = map[(buf & 0x03f000) >> 12];
        *code++ = map[(buf & 0x000fc0) >> 6];
        *code++ = map[(buf & 0x00003f)];
    }

    if (s < size) {
        buf = data[s];
        if (s == size - 1) {
            *code++ = map[buf >> 2];
            *code++ = map[(buf & 0x03) << 4];
            *code++ = '=';
            *code++ = '=';
        } else {
            buf = (buf << 8) | data[++s];
            *code++ = map[buf >> 10];
            *code++ = map[(buf & 0x03c0) >> 4];
            *code++ = map[(buf & 0x000f) << 2];
            *code++ = '=';
        }
    }

    *code = 0;
}

size_t Base64_decode (const char* code, uint8_t* data) {
    static const uint8_t map[256] = {
        128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128,
        128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 62,
        128, 128, 128, 63,  52,  53,  54,  55,  56,  57,  58,  59,  60,  61,  128, 128, 128, 128, 128, 128, 128, 0,
        1,   2,   3,   4,   5,   6,   7,   8,   9,   10,  11,  12,  13,  14,  15,  16,  17,  18,  19,  20,  21,  22,
        23,  24,  25,  128, 128, 128, 128, 128, 128, 26,  27,  28,  29,  30,  31,  32,  33,  34,  35,  36,  37,  38,
        39,  40,  41,  42,  43,  44,  45,  46,  47,  48,  49,  50,  51,  128, 128, 128, 128, 128, 128, 128, 128, 128,
        128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128,
        128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128,
        128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128,
        128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128,
        128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128,
        128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128};

    uint8_t* dat_beg = data;
    uint32_t buf = 0;
    uint32_t phase = 18;
    while (*code && *code != '=') {
        uint8_t dat = map[(uint8_t)*code++];
        if (dat == 128) return -1;

        buf |= dat << phase;
        if (phase) {
            phase -= 6;
        } else {
            *data++ = buf >> 16;
            *data++ = (buf & 0x00ff00) >> 8;
            *data++ = buf & 0x0000ff;

            buf = 0;
            phase = 18;
        }
    }

    if (phase == 6) {
        *data++ = buf >> 16;
    } else if (phase == 0) {
        *data++ = buf >> 16;
        *data++ = (buf & 0x00ff00) >> 8;
    }

    return data - dat_beg;
}
