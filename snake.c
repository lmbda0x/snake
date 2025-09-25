#include "snake.h"
#include "raylib.h"
#include "rlgl.h"
#include <stdlib.h>

int randint(int limit) {
  int cells = limit / 40;
  int idx = rand() % cells;
  return idx * 40;
}

void game_init(GameState *state) {
  state->width = WIDTH;
  state->height = HEIGHT;
  Vector2 initPos = {randint(state->height), randint(state->width)};
  Vector2 fPos = {randint(state->height), randint(state->width)};
  state->score = 0;
  state->gameOver = 0;
  state->dir = 0;
  state->root = malloc(sizeof(SnakePart));
  state->root->next = NULL;
  state->root->pos = initPos;
  state->fruitPosition = fPos;
}

void render_init() { InitWindow(WIDTH, HEIGHT, "LambdaSnake"); }
void render_stop() { CloseWindow(); }

void game_clear(GameState *state) {
  while (state->root != NULL) {
    SnakePart *temp;
    temp = state->root;
    state->root = state->root->next;
    free(temp);
  }
}

void game_reset(GameState *state) { game_clear(state); }

void game_step(GameState *state, int action) {
  // Change direction to action for state for AI
  state->dir = action;

  // Go Step
  SnakePart *temp = state->root;
  while (temp != NULL) {
    if (temp->next == NULL) {
      switch (action) {
      case 0: // right
        temp->pos.x += 40.0f;
        break;
      case 1: // leftj
        temp->pos.x -= 40.0f;
        break;
      case 2: // down
        temp->pos.y += 40.0f;
        break;
      case 3: // up
        temp->pos.y -= 40.0f;
        break;
      }
      break;
    } else {
      temp->pos.x = temp->next->pos.x;
      temp->pos.y = temp->next->pos.y;
    }
    temp = temp->next;
  }
  // Get Head
  SnakePart *head = state->root;
  while (head->next != NULL) {
    head = head->next;
  }
  // Collect fruit
  if (head->pos.x == state->fruitPosition.x &&
      head->pos.y == state->fruitPosition.y) {
    state->score++;
    state->fruitPosition.x = randint(state->width);
    state->fruitPosition.y = randint(state->height);
    SnakePart *temp = createNode();
    switch (action) {
    case 0:
      temp->pos.y = state->root->pos.y;
      temp->pos.x = state->root->pos.x - 40.0f;
      break;
    case 1:
      temp->pos.y = state->root->pos.y;
      temp->pos.x = state->root->pos.x + 40.0f;
      break;
    case 2:
      temp->pos.x = state->root->pos.x;
      temp->pos.y = state->root->pos.y - 40.0f;
      break;
    case 3:
      temp->pos.x = state->root->pos.x;
      temp->pos.y = state->root->pos.y + 40.0f;
      break;
    }
    temp->next = state->root;
    state->root = temp;
  }

  // Head goes out of game
  if (head->pos.x >= state->width || head->pos.x < 0 || head->pos.y >= state->height ||
      head->pos.y < 0) {
    state->gameOver = 1;
  }

  // Snake touches itself
  SnakePart tmp = *state->root;
  while (tmp.next != NULL) {
    if (tmp.pos.x == head->pos.x && tmp.pos.y == head->pos.y) {
      state->gameOver = 1;
    }
    tmp = *tmp.next;
  }
}

void game_render(GameState *state) {
  SnakePart *head = state->root;
  while (head->next != NULL) {
    head = head->next;
  }
  Vector2 size = {40, 40};
  BeginDrawing();
  ClearBackground(RAYWHITE);
  DrawText("Hello X people :)", 260, 380, 40, LIGHTGRAY);
  DrawText(TextFormat("Score: %d", state->score), 20, 20, 20, LIGHTGRAY);
  for (int x = 0; x <= state->width; x += 40) {
    DrawLine(x, 0, x, state->height, LIGHTGRAY);
  }
  for (int y = 0; y <= state->height; y += 40) {
    DrawLine(0, y, state->width, y, LIGHTGRAY);
  }
  SnakePart *part = state->root;
  DrawRectangleV(head->pos, size, GREEN);
  while (part->next != NULL) {
    DrawRectangleV(part->pos, size, BLUE);
    part = part->next;
  }
  DrawRectangleV(state->fruitPosition, size, RED);
  EndDrawing();
}

SnakePart *createNode() {
  SnakePart *newNode = malloc(sizeof(SnakePart));
  if (!newNode) {
    exit(1);
  }
  Vector2 init = {0, 0};
  newNode->next = NULL;
  newNode->pos = init;
  return newNode;
}
