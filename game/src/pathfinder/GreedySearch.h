#pragma once
#include "Grid.h"
#include <vector>

std::vector<GridNode> GreedyBestFirstSearch(std::vector<std::vector<int>>& grid, int startX, int startY, int endX, int endY, DisplayInfo* disp_info = nullptr);
