// Copyright 2025 Zxx200611. All Rights Reserved.

#pragma once

#include <boost/multiprecision/cpp_int.hpp>
#include <boost/random.hpp>
#include <boost/random/random_device.hpp>

namespace rsa {

using boost::multiprecision::cpp_int;

class rsa_key;
class rsa_key_pub;

std::ostream& operator<< (std::ostream& os, const rsa_key& k) noexcept;
std::ostream& operator<< (std::ostream& os, const rsa_key_pub& k) noexcept;

class rsa_key_pub {
 public:
    rsa_key_pub (cpp_int n, cpp_int e) noexcept;

    cpp_int encrypt (const cpp_int& a) const noexcept;

    const cpp_int& get_n () const noexcept;
    const cpp_int& get_e () const noexcept;

    friend std::ostream& operator<< (std::ostream& os, const rsa_key_pub& k) noexcept;

    friend class rsa_key;

 private:
    const cpp_int n, e;
};

class rsa_key {
 public:
    static constexpr size_t default_len = 3072;

    explicit rsa_key (const size_t len = default_len) noexcept;

    const rsa_key_pub& get_pub () const noexcept;

    friend std::ostream& operator<< (std::ostream& os, const rsa_key& k) noexcept;

 private:
    const rsa_key_pub pub;
    const cpp_int d, p, q;
};

}  // namespace rsa
