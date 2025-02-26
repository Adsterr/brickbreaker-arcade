#ifndef BRICK_HXX
#define BRICK_HXX

#include <vector>
#include <SDL2/SDL.h>

struct Brick {
    SDL_Rect rect;
    bool alive;
};

std::vector<Brick> CreateBricks(int rows, int cols, int brickWidth, int brickHeight, int spacing, int offsetY) {
    std::vector<Brick> bricks;
    int totalWidth = (cols * brickWidth) + (cols - 1) * spacing;
    int startX = (224 * 3 - totalWidth) / 2;  // Center bricks horizontally

    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            Brick brick;
            brick.rect.x = startX + c * (brickWidth + spacing);
            brick.rect.y = offsetY + r * (brickHeight + spacing);
            brick.rect.w = brickWidth;
            brick.rect.h = brickHeight;
            brick.alive = true;
            bricks.push_back(brick);
        }
    }

    return bricks;
}

void CheckBrickCollision(std::vector<Brick>& bricks, SDL_Rect& ball, float& ballSpeedY) {
    for (Brick& brick : bricks) {
        if (brick.alive && SDL_HasIntersection(&ball, &brick.rect)) {
            brick.alive = false;
            ballSpeedY = -ballSpeedY;
            break;
        }
    }
}

#endif