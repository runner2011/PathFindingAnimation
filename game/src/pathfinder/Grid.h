#pragma once
#include <vector>


#define PATH 0
#define BLOCK 1
#define BEST_PATH 2
#define START 3
#define GOAL 4

#define VISITED 5

class GridNode
{
public:
    int x;
    int y;
    int value; // Added to store the grid value (e.g., 0 or 1)
};

struct DisplayInfo {
public:
    std::vector<GridNode> visited_order;
};

inline bool IsWalkable(int value)
{
    if (value == PATH || value == START || value == GOAL) {
        return true;
    }
    return false;
}

