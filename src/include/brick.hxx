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
    
    // Total width of all bricks plus spacing between them
    int totalWidth = (cols * brickWidth) + (cols - 1) * spacing;
    
    // Calculate the starting X position to center the bricks
    int startX = (((224 * 3) - totalWidth) / 2);

    // Loop through the rows and columns to create each brick
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            Brick brick;
            brick.rect.x = startX + c * (brickWidth + spacing);  // Position brick horizontally
            brick.rect.y = offsetY + r * (brickHeight + spacing);  // Position brick vertically
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
