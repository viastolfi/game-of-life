#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL.h>

#define WIDTH 900
#define HEIGHT 600

#define WHITE_COLOR 0xffffff
#define BLACK_COLOR 0x000000
#define GREEN_COLOR 0x00ff00
#define GRAY_COLOR  0x808080
#define CELL_WIDTH  3

void resetCells(int** matrix, int row_number, int column_number)
{
    for (int i = 0; i < row_number; ++i)
    {
        for (int j = 0; j < column_number; ++j)
        {
            matrix[i][j] = 0;
        }
    } 
}

void initMatrix(int** matrix, int row_number, int column_number)
{
    for (int i = 0; i < row_number; ++i)
    {
        for (int j = 0; j < column_number; ++j)
        {
            matrix[i][j] = rand() % 2; 
        } 
    }
}

int getAliveNeighbours(int** matrix, int x, int y, int row_number, int column_number)
{
    int alives = 0;

    for (int i = x - 1; i <= x + 1; ++i)
    {
        if (i < 0 || i >= row_number)
            continue;
        for (int j = y - 1; j <= y + 1; ++j)
        {
            if (j < 0 || j >= column_number)
                continue;
            if (x == i && j == y)
                continue;

            if (matrix[i][j] == 1)
                ++alives;
        }
    }

    return alives;
}

void updateCells(int** matrix, int row_number, int column_number)
{
    int** m = (int**)malloc(row_number * sizeof(int*));
    for (int i = 0; i < row_number; ++i) 
    {
        m[i] = (int*)malloc(column_number * sizeof(int));
        memset(m[i], 0, column_number * sizeof(int));
    }

    for (int i = 0; i < row_number; ++i)
    {
        for (int j = 0; j < column_number; ++j)
        {
            m[i][j] = 0; 
        } 
    }

    for (int i = 0; i < row_number; ++i)
    {
        for (int j = 0; j < column_number; ++j)
        {
            
            int alives_neighbours = getAliveNeighbours(matrix, i, j, row_number, column_number);
            if (alives_neighbours >= 2 && alives_neighbours <= 3 && matrix[i][j] == 1)
                m[i][j] = 1;
            
            if (alives_neighbours == 3 && matrix[i][j] == 0)
                m[i][j] = 1;
        } 
    } 

    for (int i = 0; i < row_number; ++i)
    {
        for (int j = 0; j < column_number; ++j)
        {
            if (m[i][j] != matrix[i][j])
                matrix[i][j] = m[i][j];
        }
    }

    for (int i = 0; i < row_number; ++i) 
    {
        free(m[i]);
    }
    free(m);
}

void renderGrid(SDL_Surface* surface, int row_number, int column_number, int scale)
{
    for (int i = 0; i < row_number; ++i)
    {
        SDL_Rect line = (SDL_Rect) {0, i*CELL_WIDTH*scale, WIDTH, 1};
        SDL_FillRect(surface, &line, GRAY_COLOR);
    }

    for (int j = 0; j < column_number; ++j)
    {
        SDL_Rect line = (SDL_Rect) {j*CELL_WIDTH*scale, 0, 1, HEIGHT};
        SDL_FillRect(surface, &line, GRAY_COLOR);
    }
}

void renderCell(SDL_Surface* surface, int** matrix, int row_number, int column_number, int scale)
{
    for (int i=0; i < row_number / scale; ++i)
    {
        for (int j=0; j < column_number / scale; ++j)
        {
            if (matrix[i][j])
            {
                // draw alives cells 
                SDL_Rect cell = (SDL_Rect) {j*CELL_WIDTH*scale,i*CELL_WIDTH*scale,CELL_WIDTH*scale,CELL_WIDTH*scale};
                SDL_FillRect(surface, &cell, BLACK_COLOR);
            } 
        } 
    } 
}

int main(int argc, char** argv)
{
    srand(time(NULL));
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

    initMatrix(matrix, row_number, column_number);
    
    int scale = 1;
    int simulation_pause = 1;
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
            if (event.type == SDL_KEYDOWN)
            {
                if (event.key.keysym.sym == SDLK_SPACE)
                    simulation_pause = simulation_pause == 1 ? 0 : 1;

                switch (event.key.keysym.sym)
                {
                    case SDLK_r:
                        if (simulation_pause)
                            resetCells(matrix, row_number, column_number);
                        break;
                    case SDLK_g:
                        if (simulation_pause)
                            initMatrix(matrix, row_number, column_number);
                        break;
                    case SDLK_PLUS:
                        if (scale < 5)
                            scale++;
                        break;
                    case SDLK_EQUALS:
                        if (event.key.keysym.mod & KMOD_SHIFT) {
                            if (scale < 5)
                                scale++;
                        }
                        break;
                    case SDLK_MINUS:
                        if (scale > 1)
                            scale--;
                    default:
                        // none used key pressed
                        break;
                }
            }
        }
        SDL_FillRect(surface, &clear_rect, WHITE_COLOR);

        if (!simulation_pause)
        {
            updateCells(matrix, row_number, column_number);
        }

        renderCell(surface, matrix, row_number, column_number, scale);
        renderGrid(surface, row_number, column_number, scale);
        
        SDL_UpdateWindowSurface(window);
        SDL_Delay(500);
    } while (simulation_running);

    for (int i = 0; i < row_number; ++i)
    {
        free(matrix[i]);
    }
    free(matrix);

    SDL_DestroyWindow(window);

    SDL_Quit();

    return 0;
}
