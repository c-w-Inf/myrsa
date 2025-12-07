/**
 * RSA Common Modulus Attack Simulation
 * 模拟多个用户使用相同模数n的情况
 */

#include <iostream>
#include <vector>
#include <random>
#include <cmath>
#include <algorithm>

using namespace std;

// 扩展欧几里得算法求逆元
long long extended_gcd(long long a, long long b, long long &x, long long &y) {
    if (b == 0) {
        x = 1;
        y = 0;
        return a;
    }
    
    long long x1, y1;
    long long gcd = extended_gcd(b, a % b, x1, y1);
    
    x = y1;
    y = x1 - (a / b) * y1;
    
    return gcd;
}

// 模幂运算（简单实现）
long long mod_pow(long long base, long long exponent, long long modulus) {
    long long result = 1;
    base = base % modulus;
    
    while (exponent > 0) {
        if (exponent & 1) {
            result = (result * base) % modulus;
        }
        base = (base * base) % modulus;
        exponent >>= 1;
    }
    
    return result;
}

// 模逆运算
long long mod_inverse(long long a, long long m) {
    long long x, y;
    long long gcd = extended_gcd(a, m, x, y);
    
    if (gcd != 1) {
        return -1; // 逆元不存在
    }
    
    return (x % m + m) % m;
}

// 简单的RSA密钥生成（仅用于演示）
void generate_rsa_key(long long p, long long q, long long &n, long long &e, long long &d) {
    n = p * q;
    long long phi = (p - 1) * (q - 1);
    
    // 选择e（通常为65537，这里用小值演示）
    e = 65537;
    while (e >= phi || gcd(e, phi) != 1) {
        e--;
    }
    
    // 计算d
    d = mod_inverse(e, phi);
}

// 模拟同模攻击
void common_modulus_attack() {
    cout << "================ RSA Common Modulus Attack ================" << endl;
    cout << "场景：两个用户使用相同的模数n，但不同的公钥(e1, e2)" << endl;
    cout << "攻击者拥有：n, e1, e2, c1, c2" << endl;
    cout << "目标：恢复明文m" << endl << endl;
    
    // 步骤1：选择两个大质数（为了演示使用小质数）
    long long p = 61;
    long long q = 53;
    long long n = p * q;
    
    cout << "质数 p = " << p << ", q = " << q << endl;
    cout << "公共模数 n = p * q = " << n << endl;
    cout << "欧拉函数 φ(n) = (p-1)*(q-1) = " << (p-1)*(q-1) << endl << endl;
    
    // 步骤2：为用户1生成密钥
    long long e1, d1;
    e1 = 17; // 选择与φ(n)互质的e1
    d1 = mod_inverse(e1, (p-1)*(q-1));
    
    // 步骤3：为用户2生成密钥
    long long e2, d2;
    e2 = 19; // 选择与φ(n)互质的e2
    d2 = mod_inverse(e2, (p-1)*(q-1));
    
    cout << "用户1 - 公钥: (n=" << n << ", e1=" << e1 << ")" << endl;
    cout << "用户1 - 私钥: d1=" << d1 << endl << endl;
    
    cout << "用户2 - 公钥: (n=" << n << ", e2=" << e2 << ")" << endl;
    cout << "用户2 - 私钥: d2=" << d2 << endl << endl;
    
    // 步骤4：选择明文消息
    long long m = 42; // 明文消息
    cout << "原始明文 m = " << m << endl << endl;
    
    // 步骤5：使用两个公钥分别加密同一消息
    long long c1 = mod_pow(m, e1, n); // 用户1加密
    long long c2 = mod_pow(m, e2, n); // 用户2加密
    
    cout << "加密结果：" << endl;
    cout << "c1 = m^e1 mod n = " << m << "^" << e1 << " mod " << n << " = " << c1 << endl;
    cout << "c2 = m^e2 mod n = " << m << "^" << e2 << " mod " << n << " = " << c2 << endl << endl;
    
    // 步骤6：攻击开始
    cout << "========== 开始同模攻击 ==========" << endl;
    
    // 使用扩展欧几里得算法求s和t，使得 s*e1 + t*e2 = 1
    long long s, t;
    long long gcd = extended_gcd(e1, e2, s, t);
    
    cout << "扩展欧几里得算法结果：" << endl;
    cout << "s = " << s << ", t = " << t << endl;
    cout << "验证: s*e1 + t*e2 = " << s << "*" << e1 << " + " << t << "*" << e2;
    cout << " = " << s*e1 + t*e2 << " (应为1)" << endl << endl;
    
    // 确保s和t是正确的（可能需要调整符号）
    if (s*e1 + t*e2 != 1) {
        // 如果得到-1，可以调整符号
        if (s*e1 + t*e2 == -1) {
            s = -s;
            t = -t;
            cout << "调整符号后: s = " << s << ", t = " << t << endl;
            cout << "s*e1 + t*e2 = " << s*e1 + t*e2 << endl << endl;
        } else {
            cout << "错误: 无法找到满足条件的s和t" << endl;
            return;
        }
    }
    
    // 步骤7：计算 m = (c1^s * c2^t) mod n
    // 注意：s或t可能为负数，需要处理模逆
    long long recovered_m;
    
    if (s >= 0) {
        long long part1 = mod_pow(c1, s, n);
        if (t >= 0) {
            long long part2 = mod_pow(c2, t, n);
            recovered_m = (part1 * part2) % n;
        } else {
            // t为负数，计算c2模逆的|t|次方
            long long c2_inv = mod_inverse(c2, n);
            long long part2 = mod_pow(c2_inv, -t, n);
            recovered_m = (part1 * part2) % n;
        }
    } else {
        // s为负数，计算c1模逆的|s|次方
        long long c1_inv = mod_inverse(c1, n);
        long long part1 = mod_pow(c1_inv, -s, n);
        if (t >= 0) {
            long long part2 = mod_pow(c2, t, n);
            recovered_m = (part1 * part2) % n;
        } else {
            long long c2_inv = mod_inverse(c2, n);
            long long part2 = mod_pow(c2_inv, -t, n);
            recovered_m = (part1 * part2) % n;
        }
    }
    
    cout << "攻击计算：" << endl;
    cout << "恢复的明文 m = (c1^s * c2^t) mod n" << endl;
    cout << "            = (" << c1 << "^" << s << " * " << c2 << "^" << t << ") mod " << n << endl;
    cout << "            = " << recovered_m << endl << endl;
    
    // 步骤8：验证结果
    cout << "========== 攻击结果验证 ==========" << endl;
    cout << "原始明文: " << m << endl;
    cout << "恢复明文: " << recovered_m << endl;
    
    if (m == recovered_m) {
        cout << "✓ 攻击成功！成功恢复明文。" << endl;
    } else {
        cout << "✗ 攻击失败。" << endl;
    }
    
    cout << "\n防御建议：每个用户应使用不同的模数n（不同的p和q）" << endl;
    cout << "==============================================" << endl;
}

int main() {
    common_modulus_attack();
    return 0;
}