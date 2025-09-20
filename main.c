#include "raylib.h"
#include "rlgl.h"
#include <stdlib.h>

// Rewrite this with next instead of prev
typedef struct SnakePart {
  Vector2 pos;
  struct SnakePart *next;
} SnakePart;

SnakePart *createNode();

int main(void) {
  const int screenWidth = 800;
  const int screenHeight = 800;
  InitWindow(screenWidth, screenHeight, "LambdaSnake");
  SnakePart *root = malloc(sizeof(SnakePart));
  Vector2 initPos = {(float)screenHeight / 2 - 40, (float)screenWidth / 2 - 40};
  int dir = 0;
  root->pos = initPos;
  root->next = NULL;
  int n1 = rand() % 800 + 1;
  int n2 = rand() % 800 + 1;
  Vector2 fruitPosition = {(n1 - n1 % 40), (n2 - n2 % 40)};
  Vector2 objSize = {40, 40};
  int snakeLength = 1;
  int frameCount = 0;
  int gameOver = 0;
  char *endMsg;
  SetTargetFPS(60);

  while (!WindowShouldClose()) {
    // Get head of the snake
    SnakePart *head = root;
    while (head->next != NULL) {
      head = head->next;
    }
    // every 5 frames move all parts
    if (frameCount % 5 == 0) {
      SnakePart *temp = root;
      while (temp != NULL) {
        if (temp->next == NULL) {
          switch (dir) {
          case 0:
            temp->pos.x += 40.0f;
            break;
          case 1:
            temp->pos.x -= 40.0f;
            break;
          case 2:
            temp->pos.y += 40.0f;
            break;
          case 3:
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
    }

    if (IsKeyPressed(KEY_RIGHT) && dir != 1)
      dir = 0;
    if (IsKeyPressed(KEY_LEFT) && dir != 0)
      dir = 1;
    if (IsKeyPressed(KEY_DOWN) && dir != 3)
      dir = 2;
    if (IsKeyPressed(KEY_UP) && dir != 2)
      dir = 3;

    // Snake head collides with fruit
    if (head->pos.x == fruitPosition.x && head->pos.y == fruitPosition.y) {
      snakeLength++;
      int n1 = rand() % 800 + 1;
      int n2 = rand() % 800 + 1;
      fruitPosition.x = (n1 - n1 % 40);
      fruitPosition.y = (n2 - n2 % 40);
      SnakePart *temp = createNode();
      switch (dir) {
      case 0:
        temp->pos.y = root->pos.y;
        temp->pos.x = root->pos.x - 40.0f;
        break;
      case 1:
        temp->pos.y = root->pos.y;
        temp->pos.x = root->pos.x + 40.0f;
        break;
      case 2:
        temp->pos.x = root->pos.x;
        temp->pos.y = root->pos.y - 40.0f;
        break;
      case 3:
        temp->pos.x = root->pos.x;
        temp->pos.y = root->pos.y + 40.0f;
        break;
      }
      temp->next = root;
      root = temp;
    }

    // Head goes out of game
    if (head->pos.x >= 800 || head->pos.x < 0 || head->pos.y >= 800 ||
        head->pos.y < 0) {
      endMsg = "out of map";
      gameOver = 1;
    }

    SnakePart tmp = *root;
    while (tmp.next != NULL) {
      if (tmp.pos.x == head->pos.x && tmp.pos.y == head->pos.y) {
        gameOver = 1;
      }
      endMsg = "hit body";
      tmp = *tmp.next;
    }

    BeginDrawing();
    ClearBackground(RAYWHITE);
    if (gameOver == 0) {
      DrawText("Hello X people :)", 260, 380, 40, LIGHTGRAY);
      DrawText(TextFormat("Score: %d", snakeLength - 1), 20, 20, 10, BLACK);
      rlPushMatrix();
      rlTranslatef(0, 25 * 40, 0);
      rlRotatef(90, 1, 0, 0);
      DrawGrid(80, 40);
      rlPopMatrix();
      SnakePart *part = root;
      DrawRectangleV(head->pos, objSize, GREEN);
      while (part->next != NULL) {
        DrawRectangleV(part->pos, objSize, BLUE);
        part = part->next;
      }
      DrawRectangleV(fruitPosition, objSize, RED);
    } else {
      DrawText("Game Over!", 260, 380, 40, LIGHTGRAY);
      DrawText(endMsg, 260, 450, 40, LIGHTGRAY);
      DrawText(TextFormat("Final Score: %d", snakeLength - 1), 260, 330, 20,
               LIGHTGRAY);
    }
    EndDrawing();
    frameCount++;
  }

  while (root != NULL) {
    SnakePart *temp;
    temp = root;
    root = root->next;
    free(temp);
  }
  CloseWindow();
  return 0;
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
