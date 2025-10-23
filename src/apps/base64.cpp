// Copyright 2025 c-w-Inf. All Rights Reserved.

#include "base64.hpp"

#include <iostream>
#include <sstream>

int main (int argc, char** argv) {
    if (argc == 1) {
        std::vector<uint8_t> data;
        std::copy (std::istreambuf_iterator<char> (std::cin), std::istreambuf_iterator<char> (),
                   std::back_inserter (data));
        std::cout << rsa::base64::encode (data);
    } else if (argc == 2) {
        if (argv[1] == std::string ("-d")) {
            std::stringstream ss;
            ss << std::cin.rdbuf ();
            std::vector<uint8_t> data = rsa::base64::decode (ss.str ());
            std::cout.write (reinterpret_cast<const char*> (data.data ()), data.size ());
        } else {
            std::cout << "base64: unknown option: " << argv[1] << std::endl;
        }
    } else {
        std::cout << "usage: base64 [-d]" << std::endl;
    }

    return 0;
}
