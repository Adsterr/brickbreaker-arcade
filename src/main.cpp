#include "include/window.hxx"
#include "include/renderer.hxx"
#include "include/ball.hxx"
#include "include/paddle.hxx"
#include "include/brick.hxx"
#include "include/menu.hxx"
#include "include/stopwatch.hxx"

#include <cmath>
#include <vector>
#include <string>

//Define utiliyt functions
void ClearBricks(std::vector<Brick>& bricks);

int main() {
    // Initialize SDL window and renderer
    SDL_Window *window = CreateWindow();
    SDL_Renderer *renderer = CreateRenderer(window);

    // Initialize SDL_ttf for font rendering
    if (TTF_Init() == -1) {
        printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
        return -1;
    }

    // Load font for displaying text
    TTF_Font *font = TTF_OpenFont("fonts/PressStart2P-Regular.ttf", 24);
    if (!font) {
        printf("Failed to load font! SDL_ttf Error: %s\n", TTF_GetError());
        return -1;
    }

    SDL_Event e;
    bool running = true;

    Stopwatch time;

    // ~~~~~~~~~~~~~~~~~~~~~~
    // |    PADDLE CODE     |
    // ~~~~~~~~~~~~~~~~~~~~~~
    Paddle paddle(WINDOW_WIDTH / 2 - 40, WINDOW_HEIGHT - 25, 115, 15, 5);

    // ~~~~~~~~~~~~~~~~~~~~~~
    // |     BALL CODE      |
    // ~~~~~~~~~~~~~~~~~~~~~~
    Ball ball(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, 20, 20, 4.0f, -4.0f);

    // ~~~~~~~~~~~~~~~~~~~~~~
    // |    BRICK CODE      |
    // ~~~~~~~~~~~~~~~~~~~~~~
    int score = 0;
    int xSpacing = 15, ySpacing = 20;
    std::vector<Brick> bricks = CreateBricks(3, 12, xSpacing, ySpacing);

    int lastBrickSpawnTime = 0; // Keep track of the last time bricks were added


    bool gameStarted = false;

    while (running) {
        // ~~~~~~~~~~~~~~~~~~~~~~
        // |   EVENT HANDLING   |
        // ~~~~~~~~~~~~~~~~~~~~~~
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) running = false;
            else if (e.type == SDL_KEYDOWN) {
                if (e.key.keysym.sym == SDLK_RETURN && !gameStarted) gameStarted = true;
                else if (e.key.keysym.sym == SDLK_ESCAPE) running = false;
            }
        }

        // ~~~~~~~~~~~~~~~~~~~~~~
        // |   RENDER OBJECTS   |
        // ~~~~~~~~~~~~~~~~~~~~~~
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        if (!gameStarted) {
            DrawText(renderer, font, "Press ENTER to Start", WINDOW_WIDTH / 2 - 50, WINDOW_HEIGHT / 2);
            DrawText(renderer, font, "Press ESC to Quit", WINDOW_WIDTH / 2 - 50, WINDOW_HEIGHT / 2 + 30);
        } else {
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_RenderFillRect(renderer, &paddle.rect);
            SDL_RenderFillRect(renderer, &ball.rect);
            
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
            for (const auto &brick : bricks) {
                if (brick.alive) SDL_RenderFillRect(renderer, &brick.rect);
            }
            
            DrawText(renderer, font, "Score: " + std::to_string(score), 69, 10);
        }

        SDL_RenderPresent(renderer);

        if (gameStarted) {
            // ~~~~~~~~~~~~~~~~~~~~~~
            // |   PADDLE MOVEMENT  |
            // ~~~~~~~~~~~~~~~~~~~~~~
            const Uint8 *keystate = SDL_GetKeyboardState(NULL);
            if (keystate[SDL_SCANCODE_RIGHT]) paddle.MoveRight(WINDOW_WIDTH);
            if (keystate[SDL_SCANCODE_LEFT]) paddle.MoveLeft();

            // ~~~~~~~~~~~~~~~~~~~~~~
            // |   BALL MOVEMENT    |
            // ~~~~~~~~~~~~~~~~~~~~~~
            ball.Move();
            ball.BounceOffWalls(WINDOW_WIDTH);
            ball.CheckPaddleCollision(paddle.rect, ball.speedY, ball.speedX);

            // ~~~~~~~~~~~~~~~~~~~~~~
            // |  BRICK COLLISION   |
            // ~~~~~~~~~~~~~~~~~~~~~~
            bool hit = CheckBrickCollision(bricks, ball.rect, ball.speedY);
            score += hit ? 10 : 0;

            // Reset game if ball falls below screen
            if (ball.rect.y >= WINDOW_HEIGHT) {
                ball.Reset(WINDOW_WIDTH, WINDOW_HEIGHT);
                score = 0;
                ClearBricks(bricks);
                bricks = CreateBricks(3, 12, 15, 20);
            }

            // ~~~~~~~~~~~~~~~~~~~~~~
            // |  Layer Adding      |
            // ~~~~~~~~~~~~~~~~~~~~~~

            // ~~~~~~~ Fix: Add Bricks Every 5 Secs ~~~~~~~
            int elapsed = time.GetElapsedTime();
            if (elapsed - lastBrickSpawnTime >= 5) {
                ADD_BRICK_ROW_ON_NEW_THREAD(bricks);
                AddBrickRow(bricks, 12, xSpacing, ySpacing);
                lastBrickSpawnTime = elapsed;  // Update time
            }
            

        }

        SDL_Delay(16);
    }

    // Cleanup resources
    CleanupAnimationThread();
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}

// ~~~~~~~~~~~~~~~~~~~~~~
// |  UTILITY FUNCTION  |
// ~~~~~~~~~~~~~~~~~~~~~~
void ClearBricks(std::vector<Brick>& bricks) {
    bricks.clear();
}
