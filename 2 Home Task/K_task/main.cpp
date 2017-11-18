#include <iostream>

long long mod = 1'000'000'007ll;

int main() {
    long long n;
    std::cin >> n;
    if (n == 1) {
        std::cout << 1;
        return 0;
    }
    long long xxx[n + 1];
    long long xx[n + 1];
    long long** cnk = new long long*[n + 1];
    for (int i = 0; i < n+1; ++i) {
        cnk[i] = new long long[n+1];
    }
    long long  answer[n + 1];
    cnk[0][0] = 1;
    cnk[1][0] = 1;
    cnk[0][1] = 1;
    cnk[1][1] = 1;
    xxx[1] = 1;
    xx[0] = 1;
    for (int i = 0; i < n + 1; ++i) {
        cnk[0][i] = 1;
        cnk[i][0] = 1;
    }
    for (int i = 2; i <= n; ++i) {
        cnk[i][0] = 1;
        cnk[i][i] = 1;
        for (int j = 1; j < i; ++j) {
            cnk[i][j] = cnk[i - 1][j - 1] + cnk[i - 1][j];
            cnk[i][j] %= mod;
        }
    }
    for (int i = 1; i < n + 1; ++i) {
        xx[i] = 2 * xx[i-1];
        xx[i] %= mod;
    }
    for (int i = 2; i < n + 1; ++i) {
        xxx[i] = xxx[i-1] * xx[i - 1];
        xxx[i] %= mod;
    }
    answer[0] = 0;
    for (int i = 1; i < n + 1; ++i) {
        answer[i] = 0;
        for (int j = 1; j < i; ++j) {
            long long x = ((cnk[i - 1][j - 1]* xxx[i-j] ) % mod * answer[j]);
            answer[i] += x;
            answer[i] %= mod;
        }
        answer[i] = xxx[i] - answer[i] + mod;
        answer[i] %= mod;
    }
    std::cout << answer[n];
    for (int i = 0; i < n + 1; ++i) {
        delete[] cnk[i];
    }

}