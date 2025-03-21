#include "Stage.h"
#include <DxLib.h>
#include <cstdlib>  // For rand and srand
#include <ctime>    // For time-based random seed
#include <algorithm> // For std::random_shuffle (C++14 compatible)

// Constructor
Stage::Stage(GameObject* parent)
    : GameObject(parent, "Stage"),
    maze(HEIGHT, std::vector<int>(WIDTH, 1)) // Initialize all cells as walls
{
}

// Initialize
void Stage::Initialize() {
    srand(static_cast<unsigned int>(time(nullptr))); // Seed the random number generator
    PlaceOuterWalls();  // Surround the maze with an outer wall
    GenerateMaze();     // Generate the inner maze structure
    PlaceEntranceAndExit(); // Add entrance and exit points
}

// Update
void Stage::Update() {
    // Exit the program if the Escape key is pressed
    if (CheckHitKey(KEY_INPUT_ESCAPE)) {
        DxLib_End(); // End DXLib
        exit(0);
    }
}

// Draw
void Stage::Draw() {
    int cellSizeX = 1280 / WIDTH; // Calculate cell width
    int cellSizeY = 720 / HEIGHT; // Calculate cell height

    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            if (maze[y][x] == 1) {
                // Draw walls with a frame (red fill + black border)
                DrawBox(x * cellSizeX, y * cellSizeY,
                    (x + 1) * cellSizeX, (y + 1) * cellSizeY,
                    GetColor(255, 0, 0), TRUE); // Red fill
                DrawBox(x * cellSizeX, y * cellSizeY,
                    (x + 1) * cellSizeX, (y + 1) * cellSizeY,
                    GetColor(0, 0, 0), FALSE); // Black border
            }
            else {
                // Draw paths with a frame (gray fill + black border)
                DrawBox(x * cellSizeX, y * cellSizeY,
                    (x + 1) * cellSizeX, (y + 1) * cellSizeY,
                    GetColor(50, 50, 50), TRUE); // Gray fill
                DrawBox(x * cellSizeX, y * cellSizeY,
                    (x + 1) * cellSizeX, (y + 1) * cellSizeY,
                    GetColor(0, 0, 0), FALSE); // Black border
            }
        }
    }
}

// Release
void Stage::Release() {
    // Add cleanup logic here if necessary
}

// Add Outer Walls (Surround the Maze)
void Stage::PlaceOuterWalls() {
    // Top and bottom walls
    for (int x = 0; x < WIDTH; x++) {
        maze[0][x] = 1;              // Top wall
        maze[HEIGHT - 1][x] = 1;     // Bottom wall
    }

    // Left and right walls
    for (int y = 0; y < HEIGHT; y++) {
        maze[y][0] = 1;              // Left wall
        maze[y][WIDTH - 1] = 1;      // Right wall
    }
}

// Generate Maze (Random Walk Algorithm)
void Stage::GenerateMaze() {
    // Start at a random position within the grid
    int startX = 1;
    int startY = 1;
    maze[startY][startX] = 0; // Starting point is a path

    // Directions for movement: {dx, dy}
    std::vector<std::pair<int, int>> directions = { {0, -2}, {0, 2}, {-2, 0}, {2, 0} };

    // Create a stack for cells to process (depth-first search)
    std::vector<std::pair<int, int>> stack;
    stack.push_back(std::make_pair(startX, startY));

    while (!stack.empty()) {
        std::pair<int, int> current = stack.back();
        int x = current.first;
        int y = current.second;
        stack.pop_back();

        // Shuffle directions for randomization
        std::random_shuffle(directions.begin(), directions.end());

        for (const auto& dir : directions) {
            int dx = dir.first;
            int dy = dir.second;
            int nx = x + dx; // New x position
            int ny = y + dy; // New y position

            // Check if the new position is within bounds and is still a wall
            if (ny > 0 && ny < HEIGHT - 1 && nx > 0 && nx < WIDTH - 1 && maze[ny][nx] == 1) {
                // Make the path
                maze[ny][nx] = 0;
                maze[y + dy / 2][x + dx / 2] = 0; // Remove the wall between
                stack.push_back(std::make_pair(nx, ny)); // Add new cell to the stack
            }
        }
    }
}

// Add Entrance and Exit
void Stage::PlaceEntranceAndExit() {
    maze[1][1] = 0;                  // Entrance inside the outer walls
    maze[HEIGHT - 2][WIDTH - 2] = 0; // Exit inside the outer walls
}
