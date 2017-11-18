#include <iostream>
#include <string>
#include <algorithm>
#include <map>
int INF = 999999;

int main() {
    std::string s, t;
    s = "yxxyzyzx";
    t = "yxxyzxyzxyxzx";
    int lent = t.length();
    int a[lent][lent];
    for (int l = 0; l < lent; ++l) {
        for (int i = 0; i < lent; ++i) {
            a[l][i] = 0;
        }
    }
    for(int i = 0; i < t.length(); ++i) {
        for(int j = i; j < t.length(); ++j) {
            int mas[j - i + 1][s.length()];
            if (s[0] == t[i])
                mas[0][0] = 1;
            for (int k = 1; k < s.length(); ++k) {
                if (s[k] == t[i])
                    mas[0][k] = std::max(1, mas[0][k-1]);
                else
                    mas[0][k] = std::max(0, mas[0][k-1]);
            }
            for(int x = 0; x < j - i; ++x) {
                for (int k = 0; k < s.length() - 1; ++k) {
                    mas[x + 1][k] = std::max(mas[x][k], mas[x + 1][k]);
                    mas[x][k + 1] = std::max(mas[x][k], mas[x][k + 1]);
                    if(s[x + i + 1] == t[k + 1])
                        mas[x + 1][k + 1] = mas[x][k] + 1;
                }
                mas[x + 1][s.length() - 1] = mas[x][s.length() - 1];
            }
            int ma = -1;
            for(int k = 0; k < s.length() - 1; ++k) {
                if (ma < mas[j-i][k])
                    ma = mas[j-i][k];
            }
            a[i][j] = ma;
        }
    }
    for (int k = 0; k < lent; ++k) {
        for (int i = 0; i < lent; ++i) {
            std::cout << a[k][i] << ' ' ;
        }
        std::cout << '\n';
    }
    return 0;
}