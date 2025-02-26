#ifndef BALL_HXX
#define BALL_HXX

#include <SDL2/SDL.h>
#include <cmath>

struct Ball {
    SDL_Rect rect;
    float speedX, speedY;

    Ball(int x, int y, int width, int height, float initialSpeedX, float initialSpeedY)
        : speedX(initialSpeedX), speedY(initialSpeedY) {
        rect = {x, y, width, height};
    }

    void Move() {
        rect.x += static_cast<int>(speedX);
        rect.y += static_cast<int>(speedY);
    }

    void BounceOffWalls(int windowWidth, int windowHeight) {
        // Bounce off left/right walls
        if (rect.x <= 0 || rect.x + rect.w >= windowWidth) {
            speedX = -speedX;
        }

        // Bounce off top wall
        if (rect.y <= 0) {
            speedY = -speedY;
        }
    }

    void CheckPaddleCollision(SDL_Rect& paddle, float& ballSpeedY, float& ballSpeedX) {
        if (rect.y + rect.h >= paddle.y && rect.y + rect.h <= paddle.y + paddle.h &&
            rect.x + rect.w >= paddle.x && rect.x <= paddle.x + paddle.w) {

            ballSpeedY = -fabs(ballSpeedY);
            // Determine bounce angle based on where it hit the paddle
            float hitPosition = (rect.x + rect.w / 2.0f) - (paddle.x + paddle.w / 2.0f);
            float maxBounceAngle = 60.0f * (M_PI / 180.0f);
            float bounceAngle = (hitPosition / (paddle.w / 2.0f)) * maxBounceAngle;

            ballSpeedX = 5.0f * sin(bounceAngle);
            ballSpeedY = -5.0f * cos(bounceAngle);
        }
    }

    void Reset(int windowWidth, int windowHeight) {
        rect.x = windowWidth / 2;
        rect.y = windowHeight / 2;
        speedX = 4.0f;
        speedY = -4.0f;
    }
};

#endif
