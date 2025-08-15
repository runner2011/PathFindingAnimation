#include "pathfinder/Grid.h"
#include <vector>

using namespace std;

//class vector; // can i do this?


// Print Grid with Characters
void PrintGrid(const vector<vector<int>>& grid);

// Mark the start and end node of the path
void ModifyGridWithPath(vector<vector<int>>& grid, const vector<GridNode>& path);