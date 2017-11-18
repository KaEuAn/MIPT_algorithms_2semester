#include <iostream>
constexpr int MODULUS = 1000000007;
#include "ahalaimahalai.h"
#include <vector>


bool condition(size_t m, size_t mask, size_t newMask) {
    if (mask & newMask)
        return false;
    mask += newMask;
    for (size_t i = 0; i < m;) {
        if (mask & (1 << i))
            ++i;
        else if (i + 1 == m || mask & 1 << (i + 1))
            return false;
        else
            i += 2;
    }
    return true;
}


long long test(size_t m, size_t n) {
    std::vector<std::vector<bool>> is_right(1 << m, std::vector<bool>(1 << m, false));
    for (int i = 0; i < 1 << m; ++i) {
        for (int j = 0; j < 1 << m; ++j) {
            if (condition(m, i, j))
                is_right[i][j] = true;
        }
    }
    std::vector<std::vector<long long>> a(n + 1, std::vector<long long>(1 << m, 0));
    a[0][0] = 1;
    for (int i = 1; i < n + 1; ++i) {
        for (int mask = 0; mask < 1 << m; ++mask) {
            for (int newMask = 0; newMask < 1 << m; ++newMask) {
                a[i][newMask] += (a[i-1][mask] * is_right[mask][newMask]) % MODULUS;
                a[i][newMask] %= MODULUS;
            }
        }
    }
    return a[n][0];
}

int main() {
    constexpr int m = 6;
    constexpr int n = 30;
    std::cout<< test(m, n) << '\n';
    std::cout << AhalaiMahalai<m, n>::value << '\n';
    std::cout << AhalaiMahalai<n, m>::value << '\n';
    std::cout << AhalaiMahalai<m - 1, n>::value << '\n';
    std::cout << AhalaiMahalai<n - 1, m - 2>::value << '\n';
    return 0;
}

