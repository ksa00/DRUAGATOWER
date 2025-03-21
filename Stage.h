

#include "Engine/GameObject.h"
#include <vector>

class Stage : public GameObject {
public:
     Stage(GameObject* parent);

    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;

private:
    static const int WIDTH = 31;       // Maze width (odd number for walls/paths)
    static const int HEIGHT = 21;      // Maze height (odd number for walls/paths)
    std::vector<std::vector<int>> maze; // Maze grid (0 = path, 1 = wall)

    void GenerateMaze();              // Generates the maze structure
    void PlaceOuterWalls();           // Adds a wall boundary around the maze
    void PlaceEntranceAndExit();      // Adds the entrance and exit points
};

