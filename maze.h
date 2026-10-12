#pragma once
#include <vector>
#include <random>
#include <utility>
#include <cstdlib>
#include <queue>
#include <cstdint>

inline std::vector<std::vector<uint8_t>> generateMap(int width, int height) {
    // width/height check
    if (width < 7 || height < 7) {
        width = 7;
        height = 7;
    }

    // resulting maze
    std::vector<std::vector<uint8_t>>
        maze(height, std::vector<uint8_t>(width, 1));
    maze[1][1] = 0;

    // random number generator
    std::mt19937 rng(std::random_device{}());

    // used to check if a space is allowed
    int moveX[4] = {2, -2, 0, 0};
    int moveY[4] = {0, 0, 2, -2};

    int x = 1;
    int y = 1;
    std::vector<std::pair<int, int>> prevPos;
    prevPos.push_back({1, 1});
    while (prevPos.size() > 0) {
        // stores possible moves
        std::vector<int> possibleMoves;

        // moves x and y to the last known position if it got stuck
        x = prevPos.back().first;
        y = prevPos.back().second;

        // checks if movement in any direction is allowed, adds to the the
        // vector if it is
        for (int i = 0; i < 4; i++) {
            int dx = x + moveX[i];
            int dy = y + moveY[i];

            if (dx > 0 && dx < width - 1 && dy > 0 && dy < height - 1 &&
                maze[dy][dx] == 1) {
                possibleMoves.push_back(i);
            }
        }
        // if statement ensures dividing by 0 wont cause an error
        if (possibleMoves.size() > 0) {
            // gets a random position in possible moves
            int randMove = rng() % possibleMoves.size();
            int direction = possibleMoves[randMove];

            // sets the new positions to 0 and adds them to the list with prev
            // positions
            maze[y + (moveY[direction])][x + (moveX[direction])] = 0;
            prevPos.push_back({x + (moveX[direction]), y + (moveY[direction])});

            maze[y + (moveY[direction]) / 2][x + (moveX[direction]) / 2] = 0;

        } else {
            // when theres no where else to go, move to last known position
            prevPos.pop_back();
        }
    }

    // extra escape routes from the monster
    for (int j = 0; j < (width * height) / 10; j++) {
        int randPosW = 1 + rng() % (width - 2);
        int randPosH = 1 + rng() % (height - 2);
        if ((maze[randPosH][randPosW - 1] == 0 &&
             maze[randPosH][randPosW + 1] == 0) ||
            (maze[randPosH - 1][randPosW] == 0 &&
             maze[randPosH + 1][randPosW] == 0)) {
            maze[randPosH][randPosW] = 0;
        }
    }

    // distance from player calculation
    std::vector<std::vector<int>> dist(height, std::vector<int>(width, -1)); // each blocks distance from player
    std::queue<std::pair<int, int>> q; //queue to hold squares waiting to spread
    // player 
    q.push({1, 1}); 
    dist[1][1] = 0;

    int stepX[4] = {1, -1, 0, 0};  //possible steps
    int stepY[4] = {0, 0, 1, -1};

    while (!q.empty()) {
        // current step
        int cx = q.front().first;   
        int cy = q.front().second;  
        q.pop();     

        // neighbors of current step
        for (int i = 0; i < 4; i++) {
            int nx = cx + stepX[i];
            int ny = cy + stepY[i];

            // if neighbors arent walls and the spot hasnt been visited yet, add  its distance
            if (maze[ny][nx] != 1 && dist[ny][nx] == -1) {
                dist[ny][nx] = dist[cy][cx] + 1;
                q.push({nx, ny});
                // add neighbot to queue since we know its not a wall
            }
        }

    }

    // exit
    // max distance from player
    int maxDist = 0;
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (dist[y][x] > maxDist) {
                maxDist = dist[y][x];
            }
        }
    }
    // squares that are 60% of the max distance away from the player
    std::vector<std::pair<int, int>> farSquares;
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (dist[y][x] >= maxDist * 3 / 5 && dist[y][x] > 0) {
                farSquares.push_back({x, y});
            }
        }
    }
    // pick a random square thats far enough
    std::pair<int, int> exitPos = farSquares[rng() % farSquares.size()];
    maze[exitPos.second][exitPos.first] = 4;


    // monster starting position
    std::vector<std::pair<int, int>> monsterSquares;
    std::pair<int, int> farthest = {1, 1};
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            // skips over walls
            if (dist[y][x] <= 0 || maze[y][x] == 4) {
                continue; 
            }
            //possible places 10-25 steps away from the player the monster can be
            if (dist[y][x] >= 10 && dist[y][x] <= 25) {
                monsterSquares.push_back({x, y});
            }
            //incase maze is small
            if (dist[y][x] > dist[farthest.second][farthest.first]) {
                farthest = {x, y};
            }
        }
    }
    //all possible monster positions
    std::pair<int, int> monsterPos = farthest;
    // pick random position
    if (!monsterSquares.empty()) {
        monsterPos = monsterSquares[rng() % monsterSquares.size()];
    }
    maze[monsterPos.second][monsterPos.first] = 3;

    maze[1][1] = 2;
    return maze;
}