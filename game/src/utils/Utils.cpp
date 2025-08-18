#include "Utils.h"
#include <iostream>
#include <fstream>

using namespace std;

void PrintGrid(const vector<vector<int>>& grid)
{
    char c = '.';
    for (const auto& row : grid) {
        for (int val : row) {
            switch (val)
            {
            case BLOCK:
                c = '#';
                break;
            case PATH:
                c = 'P';
                break;
            case START:
                c = 'O';
                break;
            case GOAL:
                c = 'X';
                break;
            case VISITED:
                c = 'V';
                break;
            default:
                c = '.';
                break;
            }

            cout << c << " ";
        }
        cout << endl;
    }
}

void PrintGridWithDeco(const vector<vector<int>> &grid)
{
    std::string c = ".";
    for (const auto& row : grid) {
        for (int val : row) {
            switch (val)
            {
            case BLOCK:
                c = "#";//"██";
                break;
            case PATH:
                c = "P";
                break;
            case START:
                c = "O";
                break;
            case GOAL:
                c = "X";
                break;
            case VISITED:
                c = "V";
                break;
            default:
                c = ".";
                break;
            }

            cout << c << " ";
        }
        cout << endl;
    }
}

void ModifyGridWithStep(vector<vector<int>>& grid, const DisplayInfo& display_info, int step)
{
    int x;
    int y;
    x = display_info.visited_order[step].x;
    y = display_info.visited_order[step].y;

    grid[x][y] = display_info.visited_order[step].value;
}

vector<vector<int>> ReadMap(const std::string &file_name)
{
    std::ifstream fin(file_name);
    if (!fin) {
        throw std::runtime_error("Can't open: " + file_name);
    }

    std::vector<std::vector<int>> grid;
    std::string line;

    while (std::getline(fin, line)) {
        if (line.empty()) continue;
        std::vector<int> row;
        for (char c : line) {
            if (c == '0' || c == '1') {
                row.push_back(c - '0');  // '0' → 0, '1' → 1
            }
        }
        grid.push_back(row);
    }

    return grid;
}

void ModifyGridWithPath(vector<vector<int>>& grid, const vector<GridNode>& path)
{
    int i = 0;
    for (const GridNode& node : path) {
        if (i++ == 0)
        {
            grid[node.x][node.y] = START; 
        }
        else if (i == path.size())
        {
            grid[node.x][node.y] = GOAL;
        }
        else if (node.x >= 0 && node.x < grid.size() && node.y >= 0 && node.y < grid[0].size()) {
            grid[node.x][node.y] = PATH; 
        }
    }
}