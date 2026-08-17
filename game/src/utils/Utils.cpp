#include "Utils.h"
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

namespace {

struct CellStyle {
    char character;
    const char* color;
};

constexpr const char* ANSI_RESET = "\x1b[0m";
constexpr CellStyle PATH_STYLE      = {'.', "\x1b[90m"};
constexpr CellStyle BLOCK_STYLE     = {'#', "\x1b[37m"};
constexpr CellStyle BEST_PATH_STYLE = {'P', "\x1b[31m"};
constexpr CellStyle START_STYLE     = {'O', "\x1b[32m"};
constexpr CellStyle GOAL_STYLE      = {'X', "\x1b[33m"};
constexpr CellStyle VISITED_STYLE   = {'V', "\x1b[36m"};

void WriteStyledCell(std::ostream& output, int value)
{
    CellStyle style = PATH_STYLE;
    switch (value) {
        case BLOCK:     style = BLOCK_STYLE; break;
        case BEST_PATH: style = BEST_PATH_STYLE; break;
        case START:     style = START_STYLE; break;
        case GOAL:      style = GOAL_STYLE; break;
        case VISITED:   style = VISITED_STYLE; break;
        case PATH:
        default:        break;
    }

    output << style.color << style.character << ANSI_RESET << ' ';
}

} // namespace

void PrintGrid(const vector<vector<int>>& grid)
{
    for (const auto& row : grid) {
        for (int val : row) {
            WriteStyledCell(cout, val);
        }
        cout << endl;
    }
}

void PrintGridWithDeco(const vector<vector<int>> &grid)
{
    PrintGrid(grid);
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
                row.push_back(c - '0');  // '0' → 0, '1' → 1
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
            grid[node.x][node.y] = BEST_PATH; 
        }
    }
}

std::string BuildGridStringWithDeco(const std::vector<std::vector<int>>& grid) {
    std::ostringstream oss;
    for (size_t r = 0; r < grid.size(); ++r) {
        for (size_t c = 0; c < grid[r].size(); ++c) {
            WriteStyledCell(oss, grid[r][c]);
        }
        oss << '\n';
    }
    return oss.str();
}
