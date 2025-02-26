#include "include/window.hxx"
#include "include/renderer.hxx"
#include "include/ball.hxx"
#include "include/paddle.hxx"
#include "include/brick.hxx"
#include "menu.hxx" 
#include <cmath>
#include <vector>
#include <string>



int main() {
    SDL_Window *window = CreateWindow();
    SDL_Renderer *renderer = CreateRenderer(window);

    // Initialize SDL_ttf
    if (TTF_Init() == -1) {
        printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
        return -1;
    }

    // Load font
    TTF_Font *font = TTF_OpenFont("fonts/PressStart2P-Regular.ttf", 24);
    if (font == nullptr) {
        printf("Failed to load font! SDL_ttf Error: %s\n", TTF_GetError());
        return -1;
    }

    SDL_Event e;
    bool running = true;

    // ~~~~~~~~~~~~~~~~~~~~~~
    // |    PADDLE STUFF    |
    // ~~~~~~~~~~~~~~~~~~~~~~
    SDL_Rect paddle;
    paddle.x = WINDOW_WIDTH / 2 - 40;
    paddle.y = WINDOW_HEIGHT - 25;
    paddle.w = 115;
    paddle.h = 15;

    const int PADDLE_SPEED = 5;

    // ~~~~~~~~~~~~~~~~~~~~~~
    // |     BALL STUFF     |
    // ~~~~~~~~~~~~~~~~~~~~~~
    SDL_Rect ball;
    ball.x = WINDOW_WIDTH / 2;
    ball.y = WINDOW_HEIGHT / 2;
    ball.w = 20;
    ball.h = 20;

    float ballSpeedX = 4.0f;
    float ballSpeedY = -4.0f;

    // ~~~~~~~~~~~~~~~~~~~~~~
    // |   SCORING STUFF    |
    // ~~~~~~~~~~~~~~~~~~~~~~
    int score = 0;
    int brickRowCount = 5;
    int brickColumnCount = 10;
    std::vector<SDL_Rect> bricks;

    // Create bricks
    auto createBricks = [&]() {
        bricks.clear();  // Clear existing bricks
        for (int row = 0; row < brickRowCount; ++row) {
            for (int col = 0; col < brickColumnCount; ++col) {
                SDL_Rect brick;
                brick.x = 50 * col + 15;  // Space out bricks
                brick.y = 20 * row + 20;  // Stack bricks
                brick.w = 40;
                brick.h = 10;
                bricks.push_back(brick);
            }
        }
    };

    // Initialize bricks
    createBricks();

    bool gameStarted = false;

    while (running) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                running = false;
            } else if (e.type == SDL_KEYDOWN) {
                if (e.key.keysym.sym == SDLK_RETURN && !gameStarted) {
                    gameStarted = true;  // Start the game when ENTER is pressed
                } else if (e.key.keysym.sym == SDLK_ESCAPE) {
                    running = false;  // Exit the game when ESC is pressed
                }
            }
        }

        // ~~~~~~~~~~~~~~~~~~~~~~
        // |    DRAW OBJECTS    |
        // ~~~~~~~~~~~~~~~~~~~~~~
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);  // Set background color to black
        SDL_RenderClear(renderer);  // Clear the screen first

        if (!gameStarted) {
            // Draw menu text first
            DrawText(renderer, font, "Press ENTER to Start", WINDOW_WIDTH / 2 - 50, WINDOW_HEIGHT / 2);
            DrawText(renderer, font, "Press ESC to Quit", WINDOW_WIDTH / 2 - 50, WINDOW_HEIGHT / 2 + 30);
        } else {
            // Draw paddle
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);  // White color for paddle
            SDL_RenderFillRect(renderer, &paddle);

            // Draw ball
            SDL_RenderFillRect(renderer, &ball);

            // Draw bricks
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);  // Red color for bricks
            for (const auto &brick : bricks) {
                SDL_RenderFillRect(renderer, &brick);
            }

            // Draw score
            DrawText(renderer, font, "Score: " + std::to_string(score), 69, 10);
        }

        SDL_RenderPresent(renderer);  // Update the screen

        // ~~~~~~~~~~~~~~~~~~~~~~
        // |    PADDLE MOVE     |
        // ~~~~~~~~~~~~~~~~~~~~~~
        const Uint8 *keystate = SDL_GetKeyboardState(NULL);
        if (keystate[SDL_SCANCODE_RIGHT] && paddle.x + paddle.w + PADDLE_SPEED <= WINDOW_WIDTH) {
            paddle.x += PADDLE_SPEED;
        }
        if (keystate[SDL_SCANCODE_LEFT] && paddle.x - PADDLE_SPEED >= 0) {
            paddle.x -= PADDLE_SPEED;
        }

        // ~~~~~~~~~~~~~~~~~~~~~~
        // |    BALL MOVEMENT   |
        // ~~~~~~~~~~~~~~~~~~~~~~
        ball.x += (int)ballSpeedX;
        ball.y += (int)ballSpeedY;

        // Bounce off left/right walls
        if (ball.x <= 0 || ball.x + ball.w >= WINDOW_WIDTH) {
            ballSpeedX = -ballSpeedX;
        }

        // Bounce off top wall
        if (ball.y <= 0) {
            ballSpeedY = -ballSpeedY;
        }

        // Paddle collision
        if (ball.y + ball.h >= paddle.y && ball.y + ball.h <= paddle.y + paddle.h &&
            ball.x + ball.w >= paddle.x && ball.x <= paddle.x + paddle.w) {
            
            // Ball hit the paddle, bounce up
            ballSpeedY = -fabs(ballSpeedY);

            // Determine bounce angle based on where it hit the paddle
            float hitPosition = (ball.x + ball.w / 2.0f) - (paddle.x + paddle.w / 2.0f);
            float maxBounceAngle = 60.0f * (M_PI / 180.0f);  // Convert degrees to radians
            float bounceAngle = (hitPosition / (paddle.w / 2.0f)) * maxBounceAngle;

            // Update ball speed based on angle
            ballSpeedX = 5.0f * sin(bounceAngle);
            ballSpeedY = -5.0f * cos(bounceAngle);
        }

        // ~~~~~~~~~~~~~~~~~~~~~~
        // |   BALL AND BRICK   |
        // ~~~~~~~~~~~~~~~~~~~~~~
        for (auto it = bricks.begin(); it != bricks.end(); ) {
            SDL_Rect &brick = *it;

            if (ball.x + ball.w >= brick.x && ball.x <= brick.x + brick.w &&
                ball.y + ball.h >= brick.y && ball.y <= brick.y + brick.h) {
                
                // Ball hit the brick, bounce
                ballSpeedY = -ballSpeedY;

                // Remove brick and increment score
                it = bricks.erase(it);
                score += 10;
            } else {
                ++it;
            }
        }

        // Reset if ball falls below screen
        if (ball.y >= WINDOW_HEIGHT) {
            ball.x = WINDOW_WIDTH / 2;
            ball.y = WINDOW_HEIGHT / 2;
            ballSpeedX = 4.0f;
            ballSpeedY = -4.0f;
            score = 0;  // Reset score

            // Recreate the bricks
            createBricks();
        }

        SDL_Delay(16); // ~60 FPS
    }

    // Cleanup
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
