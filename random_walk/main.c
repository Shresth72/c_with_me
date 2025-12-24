#include <SDL2/SDL.h>
#include <math.h>
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

typedef struct {
  unsigned char r, g, b;
} Color;

Color hsv_to_rgb(float h, float s, float v);

Velocity get_rand_v(void) {
  static const Velocity dirs[4] = {{0, -1}, {0, 1}, {-1, 0}, {1, 0}};
  return dirs[rand() % 4];
}

void move_agent(SDL_Surface *psurface, Agent *pagent) {
  Velocity v = get_rand_v();
  for (int i = 0; i < SCALE; i++) {
    pagent->x += v.vx;
    pagent->y += v.vy;
    SDL_Rect rect = (SDL_Rect){pagent->x, pagent->y, AGENT_SIZE, AGENT_SIZE};
    SDL_FillRect(psurface, &rect, pagent->color);
  }
}

void create_agents(Agent *pagents, int num_agents) {
  float hue_step = 360.0f / num_agents;
  float hue_offset = rand() % 360;

  for (int i = 0; i < num_agents; i++) {
    float hue = fmodf(hue_offset + i * hue_step, 360.0f);
    Color c = hsv_to_rgb(hue, 0.9f, 0.95f);

    pagents[i] = (Agent){WIDTH / 2, HEIGHT / 2, (c.r << 16) | (c.g << 8) | c.b};
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

  Agent *pagents = calloc(num_agents, sizeof(Agent));

  create_agents(pagents, num_agents);

  int app_running = 1;
  while (app_running) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        app_running = 0;
      }

      for (int i = 0; i < num_agents; i++) {

        move_agent(psurface, &pagents[i]);
      }

      SDL_UpdateWindowSurface(pwindow);
      SDL_Delay(20);
    }
  }

  return 0;
}

Color hsv_to_rgb(float h, float s, float v) {
  float c = v * s;
  float x = c * (1 - fabsf(fmodf(h / 60.0f, 2) - 1));
  float m = v - c;

  float r, g, b;
  if (h < 60) {
    r = c;
    g = x;
    b = 0;
  } else if (h < 120) {
    r = x;
    g = c;
    b = 0;
  } else if (h < 180) {
    r = 0;
    g = c;
    b = x;
  } else if (h < 240) {
    r = 0;
    g = x;
    b = c;
  } else if (h < 300) {
    r = x;
    g = 0;
    b = c;
  } else {
    r = c;
    g = 0;
    b = x;
  }

  return (Color){(unsigned char)((r + m) * 255), (unsigned char)((g + m) * 255),
                 (unsigned char)((b + m) * 255)};
}
