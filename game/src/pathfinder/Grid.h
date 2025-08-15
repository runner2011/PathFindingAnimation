#pragma once

#include <vector>

using namespace std;

class GridNode
{
public:
    int x;
    int y;
    int value; // Added to store the grid value (e.g., 0 or 1)
};

struct DisplayInfo {
public:
    vector<GridNode> visited_order;
};

#define BLOCK 1
#define PATH 2
#define START 3
#define GOAL 4

#define VISITED 5