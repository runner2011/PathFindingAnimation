import argparse
import random

def generate_maze(h=21, w=31, seed=42):
    random.seed(seed)
    # 确保奇数尺寸（墙与路交替）
    h = h if h % 2 == 1 else h + 1
    w = w if w % 2 == 1 else w + 1

    WALL, PASS = 1, 0
    grid = [[WALL]*w for _ in range(h)]

    # 起点选奇数坐标
    sr, sc = 1, 1
    grid[sr][sc] = PASS
    stack = [(sr, sc)]

    # 相邻方向向量（隔一格挖通——中间那格是墙）
    dirs = [(-2,0),(2,0),(0,-2),(0,2)]

    while stack:
        r, c = stack[-1]
        # 找还没访问的邻居（两格外是墙）
        neighbors = []
        for dr, dc in dirs:
            nr, nc = r + dr, c + dc
            if 1 <= nr < h-1 and 1 <= nc < w-1 and grid[nr][nc] == WALL:
                neighbors.append((nr, nc, r + dr//2, c + dc//2))  # 记录中间墙格

        if neighbors:
            nr, nc, wr, wc = random.choice(neighbors)
            grid[wr][wc] = grid[nr][nc] = PASS  # 打通墙与目标格
            stack.append((nr, nc))
        else:
            stack.pop()

    # 开入口与出口
    grid[1][0] = PASS
    grid[h-2][w-1] = PASS
    return grid

def show(grid):
    # 美化：墙用'██'，路用'  '
    chars = {1:'██', 0:'  '}
    lines = [''.join(chars[v] for v in row) for row in grid]
    print('\n'.join(lines))

def write_file(grid):
    chars = {1:'1', 0:'0'}
    lines = [''.join(chars[v] for v in row) for row in grid]
    return '\n'.join(lines)

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Generate a random maze.")
    parser.add_argument("--h", type=int, default=9, help="maze height (default: 9)")
    parser.add_argument("--w", type=int, default=9, help="maze width (default: 9)")
    args = parser.parse_args()

    g = generate_maze(args.h, args.w, seed=2025)
    show(g)
    maze_text = write_file(g)

    # 写入文件
    with open("maze.txt", "w", encoding="utf-8") as f:
        f.write(maze_text)
