#pragma once 

#include <vector>
#include "Grid.h"

using namespace std;

vector<GridNode> DepthFirstSearch(vector<vector<int>>& grid, int startX, int startY, int endX, int endY, DisplayInfo* disp_info = nullptr);


