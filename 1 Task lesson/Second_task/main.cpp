#include <iostream>

int main() {
    long long n = 4;
    long long modul = 1000000007;
    long long dp[100006];
    long long n_2[100006];
    n_2[1] = 2;
    for(int i = 2; i < n + 1; ++i) {
        n_2[i] = (n_2[i - 1] * 2) % modul;
    }
    dp[0] = 1;
    dp[1] = 1;
    for(int i = 2; i < n +1; ++i) {
        dp[i] = (dp[i-1] * (n_2[i - 1] - 1)) % modul;
        int anti_j = 0;
        for(int j = 1 ; j < (i-1)/2 + 1; ++j) {
            anti_j = i - 1 - j;
            dp[i] = (dp[i] + ((dp[anti_j] * (n_2[anti_j] - 1)) *(dp[j] * (n_2[j] - 1)))% modul ) %modul;
        }
    }
    std::cout << dp[n];
    return 0;
}