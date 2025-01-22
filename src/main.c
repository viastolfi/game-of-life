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

typedef struct Camera
{
    // x, y = top left corner of the displayed grid
    int x, y, scale;
}Camera;

typedef struct Canva
{
    int** matrix;
    int row_number, column_number;
    Camera camera;
}Canva;

void changeCellState(Canva canva, int x, int y)
{
    // x, y = coordinates of the click (in window context)
    // dx, dy = real coordinates on the grid
    int dx = x / (CELL_WIDTH*canva.camera.scale) + canva.camera.x; 
    int dy = y / (CELL_WIDTH*canva.camera.scale) + canva.camera.y; 

    canva.matrix[dy][dx] = canva.matrix[dy][dx] == 0 ? 1 : 0;
}

void resetCells(Canva canva)
{
    for (int i = 0; i < canva.row_number; ++i)
    {
        for (int j = 0; j < canva.column_number; ++j)
        {
            canva.matrix[i][j] = 0;
        }
    } 
}

void initMatrix(Canva canva)
{
    for (int i = 0; i < canva.row_number; ++i)
    {
        for (int j = 0; j < canva.column_number; ++j)
        {
            canva.matrix[i][j] = rand() % 2; 
        } 
    }
}

int getAliveNeighbours(Canva canva, int x, int y)
{
    int alives = 0;

    for (int i = x - 1; i <= x + 1; ++i)
    {
        if (i < 0 || i >= canva.row_number)
            continue;
        for (int j = y - 1; j <= y + 1; ++j)
        {
            if (j < 0 || j >= canva.column_number)
                continue;
            if (x == i && j == y)
                continue;

            if (canva.matrix[i][j] == 1)
                ++alives;
        }
    }

    return alives;
}

void updateCells(Canva canva)
{
    int** m = (int**)malloc(canva.row_number * sizeof(int*));
    for (int i = 0; i < canva.row_number; ++i) 
    {
        m[i] = (int*)malloc(canva.column_number * sizeof(int));
        memset(m[i], 0, canva.column_number * sizeof(int));
    }

    for (int i = 0; i < canva.row_number; ++i)
    {
        for (int j = 0; j < canva.column_number; ++j)
        {
            m[i][j] = 0; 
        } 
    }

    for (int i = 0; i < canva.row_number; ++i)
    {
        for (int j = 0; j < canva.column_number; ++j)
        {
            
            int alives_neighbours = getAliveNeighbours(canva, i, j);
            if (alives_neighbours >= 2 && alives_neighbours <= 3 && canva.matrix[i][j] == 1)
                m[i][j] = 1;
            
            if (alives_neighbours == 3 && canva.matrix[i][j] == 0)
                m[i][j] = 1;
        } 
    } 

    for (int i = 0; i < canva.row_number; ++i)
    {
        for (int j = 0; j < canva.column_number; ++j)
        {
            if (m[i][j] != canva.matrix[i][j])
                canva.matrix[i][j] = m[i][j];
        }
    }

    for (int i = 0; i < canva.row_number; ++i) 
    {
        free(m[i]);
    }
    free(m);
}

void renderGrid(SDL_Surface* surface, Canva canva)
{
    for (int i = 0; i < canva.row_number / canva.camera.scale; ++i)
    {
        SDL_Rect line = (SDL_Rect) {0, i*CELL_WIDTH*canva.camera.scale, WIDTH, 1};
        SDL_FillRect(surface, &line, GRAY_COLOR);
    }

    for (int j = 0; j < canva.column_number / canva.camera.scale; ++j)
    {
        SDL_Rect line = (SDL_Rect) {j*CELL_WIDTH*canva.camera.scale, 0, 1, HEIGHT};
        SDL_FillRect(surface, &line, GRAY_COLOR);
    }
}

void renderCell(SDL_Surface* surface, Canva canva)
{
    for (int i=0; i < canva.row_number / canva.camera.scale; ++i)
    {
        for (int j=0; j < canva.column_number / canva.camera.scale; ++j)
        {
            if (canva.matrix[i+canva.camera.y][j+canva.camera.x])
            {
                // draw alives cells 
                SDL_Rect cell = (SDL_Rect) {j*CELL_WIDTH*canva.camera.scale,i*CELL_WIDTH*canva.camera.scale,CELL_WIDTH*canva.camera.scale,CELL_WIDTH*canva.camera.scale};
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
    Camera camera = {0, 0, 1};
    int** matrix = (int**)malloc(row_number * sizeof(int*));
    for (int i = 0; i < row_number; ++i)
    {
        matrix[i] = (int*)malloc(column_number * sizeof(int)); 
        memset(matrix[i], 0, column_number * sizeof(int)); 
    }
    Canva canva = {matrix, row_number, column_number, camera};

    initMatrix(canva);
    
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
                            resetCells(canva);
                        break;
                    case SDLK_g:
                        if (simulation_pause)
                            initMatrix(canva);
                        break;
                    case SDLK_PLUS:
                        if (canva.camera.scale < 5)
                            canva.camera.scale++;
                        break;
                    case SDLK_EQUALS:
                        if (event.key.keysym.mod & KMOD_SHIFT) {
                            if (canva.camera.scale < 5)
                                canva.camera.scale++;
                        }
                        break;
                    case SDLK_MINUS:
                        if (canva.camera.scale > 1)
                        {
                            canva.camera.scale--;
                            canva.camera.x = 0;
                            canva.camera.y = 0;
                        }
                        break;
                    case SDLK_RIGHT:
                        if (canva.camera.x + canva.column_number / canva.camera.scale < canva.column_number)
                            canva.camera.x+= 3;
                        break;
                    case SDLK_LEFT:
                        if (canva.camera.x > 0)
                            canva.camera.x-= 3;
                        break;
                    case SDLK_DOWN:
                        if (canva.camera.y + canva.row_number / canva.camera.scale < canva.row_number)
                            canva.camera.y+= 3;
                        break;
                    case SDLK_UP:
                        if (canva.camera.y > 0)
                            canva.camera.y-= 3;
                        break;
                    default:
                        // none used key pressed
                        break;
                }
            }
            if (event.type == SDL_MOUSEBUTTONDOWN && simulation_pause)
            {
                changeCellState(canva, event.button.x, event.button.y);
            }
        }
        SDL_FillRect(surface, &clear_rect, WHITE_COLOR);

        if (!simulation_pause)
        {
            updateCells(canva);
        }

        renderCell(surface, canva);
        renderGrid(surface, canva);
        
        SDL_UpdateWindowSurface(window);
        SDL_Delay(500);
    } while (simulation_running);

    for (int i = 0; i < canva.row_number; ++i)
    {
        free(canva.matrix[i]);
    }
    free(canva.matrix);

    SDL_DestroyWindow(window);

    SDL_Quit();

    return 0;
}
