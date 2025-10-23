#pragma once
#include<boost/multiprecision/cpp_int.hpp>
#include <boost/random.hpp>
#include <boost/random/random_device.hpp>

using boost::multiprecision::cpp_int;
// typedef int cpp_int;

namespace rsa
{
    class rsa_key
    {
        private:
        cpp_int n,e,d;
        static const std::size_t default_len=1024;


        public:
        rsa_key(cpp_int _n=-1,cpp_int _e=-1,cpp_int _d=-1,const std::size_t len=default_len);
        cpp_int encyption(const cpp_int &a);
        rsa_key generate_public();
        void print(const char* filename);
    };
}
