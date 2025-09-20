import gymnasium as gym
import numpy as np
from gymnasium import spaces
import snake_module as GameEnv

class SnakeEnvWrapper(gym.Env):
    def __init__(self):
        super(SnakeEnvWrapper, self).__init__()
        self.env = GameEnv
        self.state = self.env.GameState()
        self.action_space = spaces.Discrete(4)
        self.observation_space = ... # has to be done later
        self.env.game_init(self.state)

    def reset(self):
        self.env.game_reset(self.state)
        return ['']

    def step(self, action):
        obs, rew, done = self.env.game_step(self.state, action)
        return obs, rew, done, {}

    def render(self):
        self.env.game_render()

snake = SnakeEnvWrapper()
