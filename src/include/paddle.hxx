#ifndef PADDLE_HXX
#define PADDLE_HXX

#include <SDL2/SDL.h>

struct Paddle {
    SDL_Rect rect;
    const int speed;

    Paddle(int x, int y, int width, int height, int paddleSpeed)
        : speed(paddleSpeed) {
        rect = {x, y, width, height};
    }

    void MoveLeft() {
        if (rect.x - speed >= 0) {
            rect.x -= speed;
        }
    }

    void MoveRight(int windowWidth) {
        if (rect.x + rect.w + speed <= windowWidth) {
            rect.x += speed;
        }
    }
};

#endif
