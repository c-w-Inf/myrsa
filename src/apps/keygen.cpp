#include <cstring>
#include <rsa_key.hpp>
#include <util.hpp>


using rsa::rsa_key, rsa::rsa_key_pub;

// keygen.exe dist/name length seed
int main (int argc, char* argv[]) {
    // rsa::keygen_mathtools::get_small_prime();
    // std::cout<<rsa::keygen_mathtools::pri_prod<<std::endl;

    // rsa::rsa_key key=rsa_key();
    // key.debug_print();
    // rsa::rsa_key_pub pub_key=key.get_pub();
    // key.print(pri_file);
    // pub_key.print(pub_file);

    int len;
    cpp_int plaintext;
    std::cin >> len >> plaintext;
    rsa::rsa_key key = rsa_key (len);
    key.debug_print ();
    rsa_key_pub key_pub = key.get_pub ();
    cpp_int ciphertext = key_pub.encrypt (plaintext);
    std::cout << "Plaintext  : " << plaintext << std::endl;
    std::cout << "Ciphertext : " << ciphertext << std::endl;
    std::cout << "Decrypted  : " << key.encrypt (ciphertext) << std::endl;
}