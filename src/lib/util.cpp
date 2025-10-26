#include <cassert>
#include <random>
#include <util.hpp>
#include <utility>
#include <vector>


namespace rsa::keygen_mathtools {
generator_type gen;
std::vector<int> Ps;
cpp_int pri_prod;

std::vector<int> euler_seive (const int& n) {
    std::vector<int> isp (n + 1, 1), pri (0);
    isp[0] = isp[1] = 0;
    for (int i = 2; i <= n; i++) {
        if (isp[i]) pri.push_back (i);

        for (decltype (pri.size ()) j = 0; j < pri.size () && i * pri[j] <= n; j++) {
            isp[i * pri[j]] = 0;
            if (i % pri[j] == 0) break;
        }
    }
    return pri;
}
void get_small_prime () {
    if (!Ps.empty ()) return;

    Ps = euler_seive (SEIVE_LIMIT);
    pri_prod = 1;
    for (int x : Ps) pri_prod *= x;
}

cpp_int quick_pow (cpp_int a, cpp_int k, cpp_int p) {
    cpp_int res = 1;
    for (cpp_int w = a; k != 0; k /= 2, w = w * w) {
        if (p != -1) w %= p;
        if (k & 1) res = res * w;
        if (p != -1) res %= p;
    }
    return res;
}

bool miller_rabin (const cpp_int& n, const cpp_int& d, int r, int a) {
    // std::cout<<"Checking "<<a<<std::endl;
    cpp_int ap = quick_pow (a, d, n);
    if (ap == 1 || ap == 0) return 1;
    for (int i = 0; i < r; i++, ap = (ap * ap) % n) {
        if (ap == n - 1) return 1;
    }
    return 0;
}
bool is_prime (const cpp_int& n) {
    if (n == 1) return 0;

    cpp_int d = n - 1;
    int r = 0;
    while (d % 2 == 0) r++, d /= 2;

    get_small_prime ();

    if (count (Ps.begin (), Ps.end (), n)) return 1;
    for (int a : Ps)
        if (n % a == 0) return 0;

    for (int a : Ps)
        if (miller_rabin (n, d, r, a) == 0) return 0;
    return 1;
}

std::pair<cpp_int, cpp_int> exgcd (cpp_int a, cpp_int b) {
    if (b == 0) return std::pair<cpp_int, cpp_int> (1, 0);
    std::pair<cpp_int, cpp_int> pr = exgcd (b, a % b);
    return std::pair<cpp_int, cpp_int> (pr.second, pr.first - (a / b) * pr.second);
}
cpp_int gcd (cpp_int a, cpp_int b) {
    if (b == 0) return a;
    return gcd (b, a % b);
}
cpp_int get_inverse (cpp_int a, cpp_int m) {
    assert (gcd (a, m) == 1);
    return exgcd (a, m).first;
}

void set_seed (unsigned int s) { gen = generator_type (s); }
cpp_int randNum (cpp_int l, cpp_int r) { return gen () % (r - l + 1) + l; }
cpp_int randPrime (cpp_int l, cpp_int r) {
    get_small_prime ();

    cpp_int R = randNum (1, pri_prod - 1);
    // cpp_int Q=randNum(l-R)

    cpp_int x = randNum (l, r);

    while (!is_prime (x)) {
        x = randNum (l, r);
        while (gcd (x, pri_prod) != 1) x = randNum (l, r);
    }
    return x;
}
}  // namespace rsa::keygen_mathtools