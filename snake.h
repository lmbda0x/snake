#ifndef SNAKE_H
#define SNAKE_H
#include "raylib.h"

#define WIDTH 800
#define HEIGHT 800

typedef struct SnakePart {
  Vector2 pos;
  struct SnakePart *next;
} SnakePart;

typedef struct GameState {
  int width;
  int height;
  int dir;
  int score;
  int gameOver;
  SnakePart *root;
  Vector2 fruitPosition;
} GameState;

void game_init(GameState *state);
void game_step(GameState *state, int action);
void game_reset(GameState *state);
void game_render(GameState *state);
void game_clear(GameState *state);
void render_init();
void render_stop();
SnakePart *createNode();
#endif
