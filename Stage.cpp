#include "Stage.h"
#include <DxLib.h>
#include <cstdlib>
#include <ctime>
#include <algorithm>

Stage::Stage(GameObject* parent)
    : GameObject(parent, "Stage"),
    maze(HEIGHT, std::vector<int>(WIDTH, 1)) {
}

void Stage::Initialize() {
    srand(static_cast<unsigned int>(time(nullptr)));
    PlaceOuterWalls();
    GenerateMaze();
    PlaceEntranceAndExit();
}

void Stage::Update() {
    if (CheckHitKey(KEY_INPUT_ESCAPE)) {
        DxLib_End();
        exit(0);
    }
}

void Stage::Draw() {
    int cellSizeX = 1280 / WIDTH;
    int cellSizeY = 720 / HEIGHT;

    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            if (maze[y][x] == 1) {
                // Walls: Strong and vibrant red
                DrawBox(x * cellSizeX, y * cellSizeY,
                    (x + 1) * cellSizeX, (y + 1) * cellSizeY,
                    GetColor(200, 0, 0), TRUE); // Vibrant red walls
                DrawBox(x * cellSizeX, y * cellSizeY,
                    (x + 1) * cellSizeX, (y + 1) * cellSizeY,
                    GetColor(0, 0, 0), FALSE); // Black border
            }
            else {
                // Pathways: Subtle but contrasting dark blue-gray
                DrawBox(x * cellSizeX, y * cellSizeY,
                    (x + 1) * cellSizeX, (y + 1) * cellSizeY,
                    GetColor(30, 30, 60), TRUE); // Dark blue-gray paths
                DrawBox(x * cellSizeX, y * cellSizeY,
                    (x + 1) * cellSizeX, (y + 1) * cellSizeY,
                    GetColor(0, 0, 0), FALSE); // Black border
            }
        }
    }
}


void Stage::Release() {
}

void Stage::PlaceOuterWalls() {
    for (int x = 0; x < WIDTH; x++) {
        maze[0][x] = 1;
        maze[HEIGHT - 1][x] = 1;
    }
    for (int y = 0; y < HEIGHT; y++) {
        maze[y][0] = 1;
        maze[y][WIDTH - 1] = 1;
    }
}

void Stage::GenerateMaze() {
    int startX = 1, startY = 1;
    maze[startY][startX] = 0;

    std::vector<std::pair<int, int>> directions = { {0, -2}, {0, 2}, {-2, 0}, {2, 0} };
    std::vector<std::pair<int, int>> stack;
    stack.push_back(std::make_pair(startX, startY));

    while (!stack.empty()) {
        std::pair<int, int> current = stack.back();
        int x = current.first, y = current.second;
        stack.pop_back();

        std::random_shuffle(directions.begin(), directions.end());

        for (const auto& dir : directions) {
            int dx = dir.first, dy = dir.second;
            int nx = x + dx, ny = y + dy;

            if (ny > 0 && ny < HEIGHT - 1 && nx > 0 && nx < WIDTH - 1 && maze[ny][nx] == 1) {
                maze[ny][nx] = 0;
                maze[y + dy / 2][x + dx / 2] = 0;
                stack.push_back(std::make_pair(nx, ny));
            }
        }
    }
}

void Stage::PlaceEntranceAndExit() {
    maze[1][1] = 0;
    maze[HEIGHT - 2][WIDTH - 2] = 0;
}
