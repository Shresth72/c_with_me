#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define WIDTH 900
#define HEIGHT 600
#define AGENT_SIZE 2

#define SCALE 10

typedef struct {
  int vx, vy;
} Velocity;

typedef struct {
  int x, y;
  Uint32 color;
} Agent;

Velocity get_rand_v(void) {
  static const Velocity dirs[4] = {{0, -1}, {0, 1}, {-1, 0}, {1, 0}};
  return dirs[rand() % 4];
}

void move_agent(SDL_Surface *psurface, Agent *pagent) {

  SDL_Rect rect = (SDL_Rect){pagent->x, pagent->y, AGENT_SIZE, AGENT_SIZE};
  Velocity v = get_rand_v();
  for (int i = 0; i < SCALE; i++) {
    pagent->x += v.vx;
    pagent->y += v.vy;
    SDL_FillRect(psurface, &rect, pagent->color);
  }
}

int main(int argc, char *argv[]) {
  int num_agents;
  if (argc == 1) {
    num_agents = 5;
  } else if (argc == 2) {
    num_agents = atoi(argv[1]);
  } else {
    printf("Usage: %s <num-of-agents>\n", argv[0]);
    return -1;
  }
  printf("Num of agents: %d\n", num_agents);

  srand(time(NULL));

  SDL_Window *pwindow =
      SDL_CreateWindow("Random Walk", SDL_WINDOWPOS_CENTERED,
                       SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, 0);
  SDL_Surface *psurface = SDL_GetWindowSurface(pwindow);

  SDL_Rect rect = (SDL_Rect){WIDTH / 2, HEIGHT / 2, 2, 2};
  SDL_Rect *pagents = calloc(num_agents, sizeof(Agent));

  Agent agent0 = (Agent){WIDTH / 2, HEIGHT / 2, 0xFF0000};

  int app_running = 1;
  while (app_running) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        app_running = 0;
      }

      move_agent(psurface, &agent0);

      SDL_UpdateWindowSurface(pwindow);
      SDL_Delay(20);
    }
  }

  return 0;
}
