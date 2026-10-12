// Console demo: generates a maze and prints it.
// Usage: maze_demo [width] [height]   (defaults to 21 x 21; use odd sizes)
#include <cstdint>
#include <cstdlib>
#include <iostream>

#include "maze.h"

int main(int argc, char** argv) {
    int width = argc > 1 ? std::atoi(argv[1]) : 21;
    int height = argc > 2 ? std::atoi(argv[2]) : width;

    auto maze = generateMap(width, height);

    for (const auto& row : maze) {
        for (uint8_t cell : row) {
            switch (cell) {
            case 0: std::cout << "  "; break;  // floor
            case 1: std::cout << "##"; break;  // wall
            case 2: std::cout << "P "; break;  // player
            case 3: std::cout << "M "; break;  // monster
            case 4: std::cout << "E "; break;  // exit
            default: std::cout << "??"; break;
            }
        }
        std::cout << '\n';
    }
    std::cout << "\nP = player   M = monster   E = exit\n";
    return 0;
}
