// menu.hxx
#ifndef MENU_HXX
#define MENU_HXX

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>

// Function to render text on the screen
void DrawText(SDL_Renderer *renderer, TTF_Font *font, const std::string &text, int x, int y) {
    // Set text color to white
    SDL_Color textColor = {255, 255, 255, 255};

    // Create a surface from the text
    SDL_Surface *textSurface = TTF_RenderText_Solid(font, text.c_str(), textColor);
    if (textSurface == nullptr) {
        printf("Failed to create text surface: %s\n", TTF_GetError());
        return;
    }

    // Create a texture from the surface
    SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    if (textTexture == nullptr) {
        printf("Failed to create text texture: %s\n", SDL_GetError());
        SDL_FreeSurface(textSurface);
        return;
    }

    // Create a rectangle to position the text
    SDL_Rect textRect = {x - (textSurface->w / 2) + 50, y, textSurface->w, textSurface->h};

    // Render the text to the screen
    SDL_RenderCopy(renderer, textTexture, nullptr, &textRect);

    // Clean up
    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);
}

void ShowMenu(SDL_Renderer *renderer, TTF_Font *font, int WINDOW_WIDTH, int WINDOW_HEIGHT) {
    // Set text color
    SDL_Color textColor = {255, 255, 255, 255};  // White text

    // Create surface and texture for "Press ENTER to Start"
    SDL_Surface *textSurface = TTF_RenderText_Solid(font, "Press ENTER to Start", textColor);
    if (textSurface == nullptr) {
        printf("Failed to create text surface: %s\n", TTF_GetError());
        return;
    }

    SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    if (textTexture == nullptr) {
        printf("Failed to create text texture: %s\n", SDL_GetError());
        SDL_FreeSurface(textSurface);
        return;
    }

    SDL_Rect textRect = {WINDOW_WIDTH / 2 - textSurface->w / 2, WINDOW_HEIGHT / 2 - textSurface->h / 2, textSurface->w, textSurface->h};
    SDL_RenderCopy(renderer, textTexture, nullptr, &textRect);  // Render the text

    // Add additional menu text if needed
}


#endif // MENU_HXX
