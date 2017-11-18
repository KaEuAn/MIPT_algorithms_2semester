#include <iostream>
#include <vector>
#include <algorithm>


std::vector<int> colors;
std::vector<std::vector<int>> graph;
std::vector<int> parents;

int start = -1;
int finish = -1;

bool dfs(int i) {
    colors[i] = 2;
    for (int j = 0; j < graph[i].size(); ++j) {
        int vertix = graph[i][j];
        if (!colors[vertix]) {
            parents[vertix] = i;
            if (dfs(vertix)) return true;
        }
        else if (colors[vertix] == 2) {
            finish = i;
            start = vertix;
            return true;
        }
    }
    colors[i] = 1;
    return false;
}

int main() {
    int n, m;
    std::cin >> n >> m;
    graph.resize(n);
    int x, y;
    for (int i = 0; i < m; ++i) {
        std::cin >> x >> y;
        graph[x - 1].push_back(y - 1);
    }
    colors.assign(n, 0);
    parents.assign(n, -1);
    for (int i = 0; i < n; ++i)
        if (colors[i] == 0 && dfs(i))
            break;
    if (start == -1)
        std::cout << "NO";
    else {
        std::cout << "YES" << "\n";
        std::vector<int> answer;
        answer.push_back(start);
        for (int i = finish; i != start; i = parents[i])
            answer.push_back(i);
        for (int i = answer.size() - 1; i >= 0; --i)
            std::cout << answer[i] + 1 << " ";
    }
}