from time import sleep
import gymnasium as gym
import numpy as np
from gymnasium import spaces
import snake_module as GameEnv

class SnakeEnvWrapper(gym.Env):
    def __init__(self):
        super(SnakeEnvWrapper, self).__init__()
        self.env = GameEnv
        self.state= self.env.GameState()
        self.action_space = spaces.Discrete(4)
        self.observation_space = ... # has to be done later
        self.env.game_init(self.state)

    def reset(self):
        self.env.game_reset(self.state)
        self.env.game_init(self.state)
        state = self.get_state()
        return state

    def step(self, action):
        obs, rew, done = self.env.do_game_step(self.state, action)
        return obs, rew, done, {}

    def get_state(self):
        state = self.env.get_game_state(self.state)
        return state

    def render_init(self):
        self.env.render_init()

    def render_stop(self):
        self.env.render_stop()

    def render(self):
        self.env.game_render(self.state)
