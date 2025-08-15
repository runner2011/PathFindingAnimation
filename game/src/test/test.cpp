#include "utils/Utils.h"
#include <iostream>
#include "pathfinder/BFS_anim.h"

#include "mini_engine.h"


using namespace std;
using namespace mini;

/*enum
0: 路
1: 墙
2：路
3：加入queue 的grid
4：visited 路
5：find path

1. 需要读map，画出 墙， 路
2. 读取queue 的grid 画出来
3. 读取 visited 的grid 画出来
*/




// ---------------- Demo game (optional) -------------------

struct DemoGame : IGame {

    vector<vector<int>> grid = {
        {0, 0, 0, 0, 0},
        {0, 1, 1, 1, 0},
        {0, 0, 0, 1, 0},
        {0, 1, 0, 0, 0},
        {0, 0, 0, 0, 0}
    };

    // Example usage
    int startX = 0, startY = 0;
    int endX = 2, endY = 4; // Define the end point

    // Display anim
    DisplayInfo display_info;
    int frame = 0;
    
    void on_init() override {
        std::cout << "[DemoGame] init \n";

        vector<GridNode> result = BreadthFirstSearch(grid, startX, startY, endX, endY, display_info);

        PrintGrid(grid);

        if (!result.empty()) {
            cout << "Shortest path:" << endl;
            for (const GridNode& node : result) {
                cout << "Node at (" << node.x << ", " << node.y << ") with value: " << node.value << endl;
            }
        }
        else {
            cout << "No path found." << endl;
        }

        std::cout << "END OF [DemoGame] init \n";
    }

    void on_update(double dt) override {
        // render_grid(grid, frame);
        cout << "frame " << frame << "\n";
        frame++;
    }

    void render_grid(vector<vector<int>>& grid, int step) {
        /*
        /* 渲染算法
        */

        ModifyGridWithStep(grid, display_info, step);
        PrintGrid(grid);

    }

    void on_shutdown() override {
        std::cout << "[DemoGame] shutdown.\n";
    }
};




int main(int argc, char** argv) 
{
    int hz = 60;
    if (argc >= 2) {
        int parsed = std::atoi(argv[1]);
        if (parsed > 0 && parsed <= 1000) hz = parsed;
    }

    EngineConfig cfg;
    cfg.target_hz = hz;

    Engine engine{cfg};
    DemoGame game;          // ⚠️ 想要纯框架？用你自己的 IGame 实现替换它即可。
    engine.run(game);

    
    // if (!result.empty()) {
    //     cout << "Shortest path:" << endl;
    //     for (const GridNode& node : result) {
    //         cout << "Node at (" << node.x << ", " << node.y << ") with value: " << node.value << endl;
    //     }

    //     // Modify the grid to display the path
    //     ModifyGridWithPath(grid, result);
    //     cout << "Grid with path:" << endl;
    //     PrintGrid(grid);
    // }
    // else {
    //     cout << "No path found." << endl;
    // }

    return 0;
}