#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <stdio.h>

#define ROWS 48
#define COLUMNS 72
#define CELL_SIZE 10

typedef enum {
    AIR,
    WALL,
    SAND
} CellType;

void update_grid(CellType grid[ROWS][COLUMNS]);

int main(int argc, char *argv[]) {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("SDL_Init failed: %s", SDL_GetError());
        return 1;
    }

    int width = CELL_SIZE * COLUMNS;
    int height = CELL_SIZE * ROWS;
    SDL_Window *window = SDL_CreateWindow("SANDbox", width, height, 0);
    if (!window) {
        SDL_Log("SDL_CreateWindow failed: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, NULL);
    if (!renderer) {
        SDL_Log("SDL_CreateRenderer failed: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    CellType grid[ROWS][COLUMNS] = {0};
    int current_type = 1;

    bool running = true;
    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                running = false;
            }

            if (event.type == SDL_EVENT_KEY_DOWN) {
                SDL_Keycode key = event.key.key;

                if (key == SDLK_1) {
                    current_type = 1;
                } else if (key == SDLK_2) {
                    current_type = 2;
                }
            }

            if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
                if (event.button.button == SDL_BUTTON_LEFT) {
                    float mouse_x = event.button.x;
                    float mouse_y = event.button.y;

                    int col = mouse_x / CELL_SIZE;
                    int row = mouse_y / CELL_SIZE;
                    
                    if (current_type == 1) {
                        grid[row][col] = WALL;
                    } else if(current_type == 2) {
                        grid[row][col] = SAND;
                    }
                }
            }
        }

        update_grid(grid);

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(renderer);
        

        for (int row = 0; row < ROWS; row++) {
            for (int col = 0; col < COLUMNS; col++) {
                SDL_FRect cell;
                cell.x = col * CELL_SIZE;
                cell.y = row * CELL_SIZE;
                cell.w = CELL_SIZE;
                cell.h = CELL_SIZE;

                if (grid[row][col] == SAND) {
                    SDL_SetRenderDrawColor(renderer, 255, 255, 0, SDL_ALPHA_OPAQUE);
                } else if (grid[row][col] == WALL){
                    SDL_SetRenderDrawColor(renderer, 120, 120, 120, SDL_ALPHA_OPAQUE);
                } else if (grid[row][col] == AIR) {
                    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
                }
                SDL_RenderFillRect(renderer, &cell);
            }
        }

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}


void update_grid(CellType grid[ROWS][COLUMNS]) {
    for (int row = ROWS - 2; row >= 0; row--) {
        for (int col = 0; col < COLUMNS; col++) {
            if (grid[row][col] == SAND) {
                if (grid[row+1][col] == AIR) {
                    grid[row+1][col] = SAND;
                    grid[row][col] = AIR;
                } else if(grid[row+1][col] == WALL) {
                    // allows one grain of sand to sit on top of the wall
                    continue;
                } else if((col + 1) < COLUMNS && grid[row+1][col+1] == AIR) {
                        grid[row+1][col+1] = SAND;
                        grid[row][col] = AIR;
                } else if((col - 1) >= 0 && grid[row+1][col-1] == AIR) {
                        grid[row+1][col-1] = SAND;
                        grid[row][col] = AIR;
                }
            }
        }
    }
}