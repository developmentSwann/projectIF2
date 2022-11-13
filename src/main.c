#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>


void SDL_ExitWithError(const char *message);

int main(int argc, char *argv[]){
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;

    if (SDL_Init(SDL_INIT_VIDEO) != 0)
        SDL_ExitWithError("Initialisation de la SDL");

    window = SDL_CreateWindow("Teeko Games | SOUHAIT, DECKER & GLOTIN", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, 0);

    if(SDL_CreateWindowAndRenderer(800,600,0,&window,&renderer) != 0)
        SDL_ExitWithError("Impossible de creer la fenetre et le renderer");

    if(SDL_SetRenderDrawColor(renderer,112,168,237, SDL_ALPHA_OPAQUE) != 0)
        SDL_ExitWithError("Impossible de changer la couleur du renderer");

    if(SDL_RenderDrawPoint(renderer, 100, 450) != 0)
        SDL_ExitWithError("Impossible de dessiner un point");

    if(SDL_RenderDrawLine(renderer, 50, 50, 500, 500) != 0)
        SDL_ExitWithError("Impossible de dessiner une ligne");

    SDL_Rect rectangle;
    rectangle.x = 300;
    rectangle.y = 300;
    rectangle.w = 200;
    rectangle.h = 100;
    if(SDL_RenderDrawRect(renderer, &rectangle) != 0)
        SDL_ExitWithError("Impossible de dessiner un rectangle");


    SDL_RenderPresent(renderer);
    SDL_Delay(5000);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return EXIT_SUCCESS;
}

void SDL_ExitWithError(const char *message){
    SDL_Log("ERREUR : %s > %s\n", message, SDL_GetError());
    SDL_Quit();
    exit(EXIT_FAILURE);
}
