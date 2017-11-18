#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

int main() {
    long long n;
    std::cin >> n;
    long long modul = 100000007ll;
    long long a_1[10] = {1,1,1,1,1,1,1,1,1,1};
    long long a_2[10];
    while(n > 1) {
        a_2[0] = a_1[0] + a_1[1];
        a_2[9] = a_1[9] + a_1[8];
        for(int i = 1; i < 9; ++i) {
            a_2[i] = a_1[i - 1] + a_1[i] + a_1[i + 1];
        }
        for(int i = 0; i < 10; ++i) {
            a_1[i] = (a_2[i]) % modul;
        }
        --n;
    }
    std::cout << std::accumulate(a_1, a_1 + 10, 0);
    return 0;
}