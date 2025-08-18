#include "BFS_anim.h"
#include <queue>
#include <algorithm>
#include <unordered_map>

vector<GridNode> BreadthFirstSearch(vector<vector<int>>& grid, int startX, int startY, int endX, int endY, DisplayInfo& disp_info)
{
    const int a = BLOCK;
    vector<GridNode> path;
    int rows = static_cast<int>(grid.size());
    int cols = static_cast<int>(grid[0].size());

    if (startX < 0 || startY < 0 || startX >= rows || startY >= cols || grid[startX][startY] == BLOCK) {
        return path; // Invalid starting point or blocked cell
    }

    vector<vector<bool>> visited(rows, vector<bool>(cols, false));
    queue<pair<int, int>> queue;
    unordered_map<int, pair<int, int>> parent; // To track the parent of each node

    auto toKey = [cols](int x, int y) { return x * cols + y; }; // Convert (x, y) to a unique key

    queue.push({ startX, startY });
    visited[startX][startY] = true;
    parent[toKey(startX, startY)] = { -1, -1 }; // Mark the start node's parent as invalid

    while (!queue.empty()) {
        pair<int, int> front = queue.front();
        queue.pop();
        int x = front.first;
        int y = front.second;

        // If the end point is reached, reconstruct the path
        if (x == endX && y == endY) {
            while (x != -1 && y != -1) {
                path.push_back({ x, y, grid[x][y] });
                auto p = parent[toKey(x, y)];
                x = p.first;
                y = p.second;
            }
            reverse(path.begin(), path.end()); // Reverse to get the path from start to end
            return path;
        }

        // Directions: up, down, left, right
        vector<pair<int, int>> directions = { {-1, 0}, {1, 0}, {0, -1}, {0, 1} };
        for (auto& dir : directions) {
            int newX = x + dir.first;
            int newY = y + dir.second;
            if (newX >= 0 && newY >= 0 && newX < rows && newY < cols && !visited[newX][newY] && grid[newX][newY] == 0) {
                visited[newX][newY] = true;
                queue.push({ newX, newY });
                parent[toKey(newX, newY)] = { x, y }; // Record the parent of the new node
                
                //for anim display
                disp_info.visited_order.push_back({ newX, newY, VISITED });
            }
        }
    }

    return path; // Return an empty path if no path is found
}