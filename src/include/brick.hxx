#ifndef BRICK_HXX
#define BRICK_HXX

#include <vector>
#include <thread>
#include <atomic>

#include <SDL2/SDL.h>

struct Brick {
    SDL_Rect rect;
    bool alive;
};

int brickWidth = 40;
int brickHeight = 10;


std::vector<Brick> CreateBricks(int rows, int cols, int spacingX, int spacingY) {
    std::vector<Brick> bricks;
    
    // Total width of all bricks plus spacing between them
    int totalWidth = (cols * brickWidth) + (cols - 1) * spacingX;
    
    // Calculate the starting X position to center the bricks
    int startX = (((224 * 3) - totalWidth) / 2);

    // Loop through the rows and columns to create each brick
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            Brick brick;
            brick.rect.x = startX + c * (brickWidth + spacingX);  // Position brick horizontally
            brick.rect.y = spacingY + r * (brickHeight + spacingX);  // Position brick vertically
            brick.rect.w = brickWidth;
            brick.rect.h = brickHeight;
            brick.alive = true;
            bricks.push_back(brick);
        }
    }

    return bricks;
}

void AddBrickRow(std::vector<Brick>& bricks, int cols, int spacingX, int spacingY) {
    int totalWidth = (cols * brickWidth) + (cols - 1) * spacingX;
    int startX = (((224 * 3) - totalWidth) / 2); // Center the bricks

    // Move all existing bricks downward
    for (Brick& brick : bricks) {
        brick.rect.y += (brickHeight + spacingY);
    }

    // New row should start below the last existing row
    int newRowY = bricks.front().rect.y - brickHeight - spacingY + 6; 

    // Add the new row at the bottom (below existing bricks)
    for (int c = 0; c < cols; c++) {
        Brick brick;
        brick.rect.x = startX + c * (brickWidth + spacingX);  // Position brick horizontally
        brick.rect.y = newRowY;
        brick.rect.w = brickWidth;
        brick.rect.h = brickHeight;
        brick.alive = true;
        bricks.insert(bricks.begin(), brick);
    }
}



std::atomic<bool> isAnimating(false);
std::thread animThread;

void MoveBrickRows(std::vector<Brick>& bricks) {
    isAnimating = true;
    for (int i = 0; i < 1; i++) { // move down by what
        for (Brick &brick : bricks) {
            brick.rect.y += 1; // this too
        }
        SDL_Delay(250);
    }
    isAnimating = false;
}

void ADD_BRICK_ROW_ON_NEW_THREAD(std::vector<Brick>& bricks) {
    if (isAnimating) return; // Prevent multiple threads

    if (animThread.joinable()) { 
        animThread.join(); // Ensure the last thread has finished
    }

    animThread = std::thread(MoveBrickRows, std::ref(bricks));
}


// Call this in your cleanup code before exiting the game
void CleanupAnimationThread() {
    if (animThread.joinable()) {
        animThread.join(); // Wait for animation to finish before exiting
    }
}


bool CheckBrickCollision(std::vector<Brick>& bricks, SDL_Rect& ball, float& ballSpeedY) {
    for (Brick& brick : bricks) {
        if (brick.alive && SDL_HasIntersection(&ball, &brick.rect)) {
            brick.alive = false;
            ballSpeedY = -ballSpeedY;
            return true;
            break;
        }
    }
    return false;
}

#endif
