// Copyright 2025 Zxx200611. All Rights Reserved.

#pragma once

#include <boost/multiprecision/cpp_int.hpp>
#include <boost/random.hpp>
#include <boost/random/random_device.hpp>
#include <random>

using boost::multiprecision::cpp_int;

namespace rsa::keygen_mathtools {
const int SEIVE_LIMIT = 20000;
extern std::vector<int> Ps;
extern cpp_int pri_prod;

typedef boost::random::independent_bits_engine<std::mt19937, 8192, cpp_int> generator_type;

std::vector<int> euler_seive (const int& n);
void get_small_prime ();

cpp_int quick_pow (cpp_int a, cpp_int k, cpp_int p = -1);

bool miller_rabin (const cpp_int& n, const cpp_int& d, int r, int a);
bool is_prime (const cpp_int& n);

std::pair<cpp_int, cpp_int> exgcd (cpp_int a, cpp_int b);
cpp_int gcd (cpp_int a, cpp_int b);
cpp_int get_inverse (cpp_int a, cpp_int m);

void set_seed (unsigned int s);
cpp_int randNum (cpp_int l, cpp_int r);
cpp_int randPrime (cpp_int l, cpp_int r);
}  // namespace rsa::keygen_mathtools

namespace base64_tools {}