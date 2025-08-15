#pragma once

class GridNode
{
public:
    int x;
    int y;
    int value; // Added to store the grid value (e.g., 0 or 1)
};

#define BLOCK 1
#define PATH 2
#define START 3
#define GOAL 4