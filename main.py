import sys
import os
import snake_module

build_path = os.path.join(os.path.dirname(__file__), "build")
sys.path.append(build_path)


# Create and initialize game state
state = snake_module.GameState()
snake_module.game_init(state)
snake_module.game_render(state)

def gan():
    tmp = state.root
    while(tmp != None):
        print(tmp.pos)
        tmp = tmp.next

while(True):
    snake_module.game_render(state)
    print(f"Fruit Position: {state.fruitPosition}")
    gan()
    action = int(input("Next step: "))
    snake_module.game_step(state, action)
    if state.gameOver == 1:
        print(f"Score: {state.score}, Game Over: {state.gameOver}")
        snake_module.game_reset(state)
        break
