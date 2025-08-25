#include "GreedySearch.h"
#include <vector>
#include <queue>
#include <cmath>
#include <utility>
#include <unordered_map>
#include <algorithm>

using namespace std;

vector<GridNode> GreedyBestFirstSearch(vector<vector<int>>& grid, int startX, int startY, int endX, int endY, DisplayInfo* disp_info) {
    vector<GridNode> path;
    int rows = static_cast<int>(grid.size());
    int cols = static_cast<int>(grid[0].size());

    auto heuristic = [&](int x, int y) {
        return abs(x - endX) + abs(y - endY); // Manhattan distance
    };

    auto toKey = [cols](int x, int y) { return x * cols + y; };

    // Initialize priority queue and tracking matrices
    priority_queue<pair<int, pair<int, int>>, vector<pair<int, pair<int, int>>>, greater<>> queue;
    vector<vector<bool>> visited(rows, vector<bool>(cols, false));
    unordered_map<int, pair<int, int>> parent;

    queue.push({heuristic(startX, startY), {startX, startY}});
    visited[startX][startY] = true;
    parent[toKey(startX, startY)] = {-1, -1};

    while (!queue.empty()) {
        auto [h, node] = queue.top();
        queue.pop();
        int x = node.first, y = node.second;

        if (x == endX && y == endY) {
            while (x != -1 && y != -1) {
                path.push_back({ x, y, grid[x][y] });
                auto p = parent[toKey(x, y)];
                x = p.first;
                y = p.second;
            }
            reverse(path.begin(), path.end());
            return path;
        }

        // Directions: up, down, left, right
        vector<pair<int, int>> directions = { {-1, 0}, {1, 0}, {0, -1}, {0, 1} };
        for (auto& dir : directions) {
            int newX = x + dir.first;
            int newY = y + dir.second;
            if (newX >= 0 && newY >= 0 && newX < rows && newY < cols && !visited[newX][newY] && IsWalkable(grid[newX][newY])) {
                visited[newX][newY] = true;
                parent[toKey(newX, newY)] = {x, y};
                queue.push({heuristic(newX, newY), {newX, newY}});

                if (disp_info) {
                    disp_info->visited_order.push_back({ newX, newY, VISITED });
                }
            }
        }
    }

    return path;
}

