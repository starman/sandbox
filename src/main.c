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

struct {
    SDL_Window *window;
    SDL_Renderer *renderer;

    CellType grid[ROWS][COLUMNS];
    int current_type;
    bool running;
} state;

void clear_grid(CellType grid[ROWS][COLUMNS]);
void update_grid(CellType grid[ROWS][COLUMNS]);

int main(int argc, char *argv[]) {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("SDL_Init failed: %s", SDL_GetError());
        return 1;
    }

    int width = CELL_SIZE * COLUMNS;
    int height = CELL_SIZE * ROWS;
    state.window = SDL_CreateWindow("SANDbox", width, height, 0);
    if (!state.window) {
        SDL_Log("SDL_CreateWindow failed: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    state.renderer = SDL_CreateRenderer(state.window, NULL);
    if (!state.renderer) {
        SDL_Log("SDL_CreateRenderer failed: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    clear_grid(state.grid);
    state.current_type = 1;

    state.running = true;
    while (state.running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                state.running = false;
            }

            if (event.type == SDL_EVENT_KEY_DOWN) {
                SDL_Keycode key = event.key.key;

                if (key == SDLK_1) {
                    state.current_type = 1;
                } else if (key == SDLK_2) {
                    state.current_type = 2;
                }
            }

            float mouse_x, mouse_y;
            Uint32 buttons = SDL_GetMouseState(&mouse_x, &mouse_y);
            if (buttons & SDL_BUTTON_LMASK) {
                int col = mouse_x / CELL_SIZE;
                int row = mouse_y / CELL_SIZE;
                
                if (row < ROWS && row >= 0 && col < COLUMNS && col >= 0) {
                    if (state.current_type == 1) {
                        state.grid[row][col] = WALL;
                    } else if(state.current_type == 2) {
                        state.grid[row][col] = SAND;
                    }
                }
            }
        }

        update_grid(state.grid);

        SDL_SetRenderDrawColor(state.renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(state.renderer);
        

        for (int row = 0; row < ROWS; row++) {
            for (int col = 0; col < COLUMNS; col++) {
                SDL_FRect cell;
                cell.x = col * CELL_SIZE;
                cell.y = row * CELL_SIZE;
                cell.w = CELL_SIZE;
                cell.h = CELL_SIZE;

                if (state.grid[row][col] == SAND) {
                    SDL_SetRenderDrawColor(state.renderer, 255, 255, 0, SDL_ALPHA_OPAQUE);
                } else if (state.grid[row][col] == WALL){
                    SDL_SetRenderDrawColor(state.renderer, 120, 120, 120, SDL_ALPHA_OPAQUE);
                } else if (state.grid[row][col] == AIR) {
                    SDL_SetRenderDrawColor(state.renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
                }
                SDL_RenderFillRect(state.renderer, &cell);
            }
        }

        SDL_RenderPresent(state.renderer);
    }

    SDL_DestroyRenderer(state.renderer);
    SDL_DestroyWindow(state.window);
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

void clear_grid(CellType grid[ROWS][COLUMNS]) {
    for (int row = 0; row < ROWS; row++) {
        for (int col = 0; col < COLUMNS; col++) {
            grid[row][col] = AIR;
        }
    }
}