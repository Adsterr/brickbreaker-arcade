#include "include/renderer.hxx"

SDL_Renderer* CreateRenderer(SDL_Window *window) {

    SDL_Renderer* renderer = SDL_CreateRenderer(
        window, 
        -1, 
        SDL_RENDERER_ACCELERATED
    );

    if (!renderer) {
        std::cerr << "Could not create renderer: " << SDL_GetError() << '\n';
        SDL_DestroyWindow(window);
        SDL_Quit();
        return nullptr;
    }

    return renderer;
}
