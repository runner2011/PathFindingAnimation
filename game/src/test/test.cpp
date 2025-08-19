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

    Engine* _engine;

    vector<vector<int>> grid = {
        {0, 0, 0, 0, 0},
        {0, 1, 1, 1, 0},
        {0, 0, 0, 1, 0},
        {0, 1, 0, 0, 0},
        {0, 0, 0, 0, 0}
    };
public:
    // Config
    bool readGridFromFile = true;

    // Example usage
    int startX = 0, startY = 0;
    int endX = 2, endY = 4;

    // Display anim
    DisplayInfo display_info;
    int frame = 0;

    // finded path
    vector<GridNode> finded_path;
    
    void on_init(Engine* engine) override {
        std::cout << "[DemoGame] init \n";

        _engine = engine;
        frame = 0;

        if (readGridFromFile) {
            grid = ReadMap("maze.txt");
        }

        finded_path = BreadthFirstSearch(grid, startX, startY, endX, endY, display_info);

        PrintGrid(grid);

        if (!finded_path.empty()) {
            cout << "Shortest path:" << endl;
            for (const GridNode& node : finded_path) {
                cout << "Node at (" << node.x << ", " << node.y << ") with value: " << node.value << endl;
            }
        }
        else {
            cout << "No path found." << endl;
        }

		// Clear Screen
        cout << "\x1b[2J";

        std::cout << "END OF [DemoGame] init \n";
    }

    void on_update(double dt) override {

		
        std::string frameStr = render_grid(grid, frame);
		//"\x1b[H": Move cursor to up-left 
        std::cout << "\x1b[H" << frameStr << std::flush;
		
        cout << "frame " << frame << "\n";
        frame++;
    }

    std::string render_grid(vector<vector<int>>& grid, int step) {
        /*
        /* 渲染算法
		/* 逐个打印所有格子
        */
        string str; 

        // Print all visited grids first. 
        // Should immediate stop print display visited order if already find Goal.
        if (step < display_info.visited_order.size() && 
        !(display_info.visited_order[step].x == endX && display_info.visited_order[step].y == endY)) { 
            ModifyGridWithStep(grid, display_info, step);
	        str = BuildGridStringWithDeco(grid);
	    }
        // Print found path
        else
        {
            cout << "Print all visited grids done! \n";
            ModifyGridWithPath(grid, finded_path);
            str = BuildGridStringWithDeco(grid);
            if (_engine)
            {
                _engine->request_quit();
            }

        }
        return str;
    }

    void on_shutdown() override {
		_engine = nullptr;
        std::cout << "[DemoGame] shutdown.\n";
    }
};




int main(int argc, char** argv) 
{
    int hz = 1;
    if (argc >= 2) {
        int parsed = std::atoi(argv[1]);
        if (parsed > 0 && parsed <= 1000) hz = parsed;
    }

    /// config 
    EngineConfig cfg;
    cfg.target_hz = hz;

    Engine engine{cfg};
    DemoGame game;
    game.readGridFromFile = true;
    if (game.readGridFromFile) {
        game.startX = 1;
        game.startY = 0;
        game.endX = 7;
        game.endY = 8;
    }

    ///

    engine.run(game);


    return 0;
}