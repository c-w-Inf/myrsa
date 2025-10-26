#include <rsa_key.hpp>
#include <util.hpp>

using rsa::rsa_key, rsa::rsa_key_pub;
using namespace rsa::keygen_mathtools;

const cpp_int& rsa_key_pub::get_n () const noexcept { return n; }
const cpp_int& rsa_key_pub::get_e () const noexcept { return e; }

cpp_int rsa_key_pub::encrypt (const cpp_int& a) const noexcept { return quick_pow (a, e, n); }
void rsa_key_pub::debug_print () const noexcept {
    std::cout << "RSA pub key *************" << std::endl;
    std::cout << "n = " << n << std::endl;
    std::cout << "e = " << e << std::endl;
    std::cout << "*************************" << std::endl;
}

rsa_key::rsa_key (const size_t len) noexcept {
    assert (len <= 8192);

    const cpp_int pql = quick_pow (2, len - 2), pqr = 2 * pql;
    p = randPrime (pql, pqr), q = p;
    while (q == p) q = randPrime (pql, pqr);
    cpp_int n = p * q;

    cpp_int phi = (p - 1) * (q - 1);
    cpp_int e = 65537 % phi;

    while (gcd (phi, e) != 1) e = randNum (2, phi - 1);
    pub = rsa_key_pub (p * q, e);
    d = (get_inverse (e, phi) % phi + phi) % phi;
}
const rsa_key_pub& rsa_key::get_pub () const noexcept { return pub; }
cpp_int rsa_key::encrypt (const cpp_int& a) const noexcept { return rsa_key_pub (pub.n, d).encrypt (a); }
void rsa_key::debug_print () const noexcept {
    std::cout << "RSA pub key *************" << std::endl;
    std::cout << "n = " << pub.n << std::endl;
    std::cout << "e = " << pub.e << std::endl;
    std::cout << "d = " << d << std::endl;
    std::cout << "p = " << p << std::endl;
    std::cout << "q = " << q << std::endl;
    std::cout << "*************************" << std::endl;
}