#include "include/window.hxx"

const int WINDOW_WIDTH = 224 * 3;
const int WINDOW_HEIGHT = 288 * 3;

SDL_Window* CreateWindow() {

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize: " << SDL_GetError() << '\n';
        return nullptr;
    }

    SDL_Window* window = SDL_CreateWindow(
        "Brick Breaker",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        SDL_WINDOW_SHOWN
    );

    if (!window) {
        std::cerr << "Could not create window: " << SDL_GetError() << '\n';
        return nullptr;
    }

    return window;
}
