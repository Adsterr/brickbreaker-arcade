#include "include/window.hxx"
#include "include/renderer.hxx"
#include "include/brick.hxx"
#include "include/ball.hxx"
#include "include/paddle.hxx"

#include <cmath>
#include <vector>

const int WINDOW_WIDTH = 224 * 3;
const int WINDOW_HEIGHT = 288 * 3;

int main() {
    SDL_Window *window = CreateWindow();
    SDL_Renderer *renderer = CreateRenderer(window);

    SDL_Event e;
    bool running = true;

    // ~~~~~~~~~~~~~~~~~~~~~~
    // |    PADDLE STUFF    |
    // ~~~~~~~~~~~~~~~~~~~~~~
    Paddle paddle(WINDOW_WIDTH / 2 - 40, WINDOW_HEIGHT - 25, 115, 15, 5);

    // ~~~~~~~~~~~~~~~~~~~~~~
    // |     BALL STUFF     |
    // ~~~~~~~~~~~~~~~~~~~~~~
    Ball ball(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, 15, 15, 4.0f, -4.0f);

    // ~~~~~~~~~~~~~~~~~~~~~~
    // |   BRICK GENERATION |
    // ~~~~~~~~~~~~~~~~~~~~~~
    int rows = 5, cols = 10, brickWidth = 50, brickHeight = 20, spacing = 5, offsetY = 50;
    std::vector<Brick> bricks = CreateBricks(rows, cols, brickWidth, brickHeight, spacing, offsetY);

    while (running) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                running = false;
            }
        }

        // ~~~~~~~~~~~~~~~~~~~~~~
        // |    PADDLE MOVE     |
        // ~~~~~~~~~~~~~~~~~~~~~~
        const Uint8 *keystate = SDL_GetKeyboardState(NULL);
        if (keystate[SDL_SCANCODE_RIGHT]) {
            paddle.MoveRight(WINDOW_WIDTH);
        }
        if (keystate[SDL_SCANCODE_LEFT]) {
            paddle.MoveLeft();
        }

        // ~~~~~~~~~~~~~~~~~~~~~~
        // |    BALL MOVEMENT   |
        // ~~~~~~~~~~~~~~~~~~~~~~
        ball.Move();
        ball.BounceOffWalls(WINDOW_WIDTH, WINDOW_HEIGHT);
        ball.CheckPaddleCollision(paddle.rect, ball.speedY, ball.speedX);

        // Brick collision
        CheckBrickCollision(bricks, ball.rect, ball.speedY);

        // Reset if ball falls below screen
        if (ball.rect.y >= WINDOW_HEIGHT) {
            ball.Reset(WINDOW_WIDTH, WINDOW_HEIGHT);
        }

        // ~~~~~~~~~~~~~~~~~~~~~~
        // |    DRAW OBJECTS    |
        // ~~~~~~~~~~~~~~~~~~~~~~
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Draw paddle
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderFillRect(renderer, &paddle.rect);

        // Draw ball
        SDL_RenderFillRect(renderer, &ball.rect);

        // Draw bricks
        for (const Brick& brick : bricks) {
            if (brick.alive) {
                SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
                SDL_RenderFillRect(renderer, &brick.rect);
            }
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(16); // ~60 FPS
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
