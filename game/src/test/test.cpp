#include "utils/Utils.h"
#include <iostream>
#include "pathfinder/DFS.h"

using namespace std;

/*enum
1: 墙
2：路
3：加入queue 的grid
4：visited 路
5：find path

1. 需要读map，画出 墙， 路
2. 读取queue 的grid 画出来
3. 读取 visited 的grid 画出来
*/

int main()
{
    vector<vector<int>> grid = {
        {0, 0, 0, 0, 0},
        {0, 1, 1, 1, 0},
        {0, 0, 0, 1, 0},
        {0, 1, 0, 0, 0},
        {0, 0, 0, 0, 0}
    };

    PrintGrid(grid);

    // Example usage
    int startX = 0, startY = 0;
    int endX = 2, endY = 4; // Define the end point
   // vector<GridNode> result = BreadthFirstSearch(grid, startX, startY, endX, endY);
    vector<GridNode> result = DepthFirstSearch(grid, startX, startY, endX, endY);
    if (!result.empty()) {
        cout << "Shortest path:" << endl;
        for (const GridNode& node : result) {
            cout << "Node at (" << node.x << ", " << node.y << ") with value: " << node.value << endl;
        }

        // Modify the grid to display the path
        ModifyGridWithPath(grid, result);
        cout << "Grid with path:" << endl;
        PrintGrid(grid);
    }
    else {
        cout << "No path found." << endl;
    }

    return 0;
}