#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

long long** perem(long long [10][10]& aa) {

}

int main() {
    long long n = 100000;
    long long modul = 1000000007;
    long long A[10][10];
    for(int i = 0; i < 10; ++i) {
        for(int j = 0; j < 10; ++j)
            A[i][j] = 0;
        A[i][i-1] = 1; A[i][i] = 1; A[i][i + 1] = 1;
    }
    A[0][0] = 1; A[0][1] = 1;
    A[9][9] = 1; A[9][8] = 1;
    long long a_1[10] = {0,1,1,1,1,1,1,1,1,1};
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
    //std::cout << std::accumulate(a_1, a_1 + 10, 0);
    long long sum = 0;
    for(int i = 0; i < 10; ++i){
        sum += a_1[i];
        sum %= modul;
    }
    std::cout << sum;
    return 0;
}