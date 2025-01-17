#include <stdio.h>
#include <SDL2/SDL.h>

#define WIDTH 900
#define HEIGHT 600
#define WHITE_COLOR 0xffffff
#define BLACK_COLOR 0x000000
#define GREEN_COLOR 0x00ff00
#define CELL_WIDTH 50

void renderGrid(SDL_Surface* surface, int row_number, int column_number)
{
    for (int i = 0; i < row_number; ++i)
    {
        SDL_Rect line = (SDL_Rect) {0, i*CELL_WIDTH, WIDTH, 1};
        SDL_FillRect(surface, &line, WHITE_COLOR);
    }

    for (int j = 0; j < column_number; ++j)
    {
        SDL_Rect line = (SDL_Rect) {j*CELL_WIDTH, 0, 1, HEIGHT};
        SDL_FillRect(surface, &line, WHITE_COLOR);
    }
}

void renderCell(SDL_Surface* surface, int** matrix, int row_number, int column_number)
{
    for (int i=0; i < row_number; ++i)
    {
        for (int j=0; j < column_number; ++j)
        {
            if (matrix[i][j])
            {
                // draw alives cells 
                SDL_Rect cell = (SDL_Rect) {i*CELL_WIDTH,i*CELL_WIDTH,CELL_WIDTH,CELL_WIDTH};
                SDL_FillRect(surface, &cell, GREEN_COLOR);
            } 
        } 
    } 
}

int main(int argc, char** argv)
{
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("Conway's game of life", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, 0);
    SDL_Surface* surface = SDL_GetWindowSurface(window);

    SDL_Rect clear_rect = (SDL_Rect) {0,0,WIDTH,HEIGHT};
    int column_number = WIDTH / CELL_WIDTH;
    int row_number = HEIGHT / CELL_WIDTH;
    
    //declaring game's matrix
    int** matrix = (int**)malloc(row_number * sizeof(int*));
    for (int i = 0; i < row_number; ++i)
    {
        matrix[i] = (int*)malloc(column_number * sizeof(int)); 
        memset(matrix[i], 0, column_number * sizeof(int));
    }

    //set a first living cell
    matrix[1][1] = 1;

    int simulation_running = 1;
    SDL_Event event;
    do 
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                simulation_running = 0; 
            } 
        }
        SDL_FillRect(surface, &clear_rect, BLACK_COLOR);

        renderCell(surface, matrix, row_number, column_number);
        renderGrid(surface, row_number, column_number);

        SDL_UpdateWindowSurface(window);
        SDL_Delay(20);
    } while (simulation_running);


    return 0;
}
