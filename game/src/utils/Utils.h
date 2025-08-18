#include "pathfinder/Grid.h"
#include <vector>

using namespace std;

//class vector; // can i do this?


// Print Grid with Characters
void PrintGrid(const vector<vector<int>>& grid);

// Print Grid with more decorative Characters
void PrintGridWithDeco(const vector<vector<int>>& grid);

// Mark the start and end node of the path
void ModifyGridWithPath(vector<vector<int>>& grid, const vector<GridNode>& path);

// get display_queue's grid value to modify grid, step by step
void ModifyGridWithStep(vector<vector<int>>& grid, const DisplayInfo& display_info, int step);