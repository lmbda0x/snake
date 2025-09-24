import random
from time import sleep
import torch
import torch.nn as nn
import torch.nn.functional as F
from collections import deque
from snake_env import SnakeEnvWrapper
import numpy as np

class QNet(nn.Module):
    def __init__(self, state_dim, action_dim):
        super().__init__()

        self.conv1 = nn.Conv2d(4, 8, 3)
        self.relu1 = nn.LeakyReLU()
        self.pool1 = nn.MaxPool2d(kernel_size=2, stride=2)

        self.conv2 = nn.Conv2d(8, 16, 3)
        self.relu2 = nn.LeakyReLU()
        self.pool2 = nn.MaxPool2d(kernel_size=2, stride=2)

        self.flatten = nn.Flatten()

        self.fc1 = nn.Linear(16*4*4, 128)
        self.relu3 = nn.LeakyReLU()
        self.out = nn.Linear(128, action_dim)
        nn.init.xavier_uniform_(self.out.weight)


    def forward(self, x):
        x = self.pool1(self.relu1(self.conv1(x)))
        x = self.pool2(self.relu2(self.conv2(x)))
        x = self.flatten(x)
        x = self.relu3(self.fc1(x))
        return self.out(x)
device = torch.device("cuda" if torch.cuda.is_available() else "cpu")
snake = SnakeEnvWrapper()
state_dim = 4*22*22
action_dim = 4
mem_buffer = deque(maxlen=300000)
scores = deque(maxlen=10)
policy_net = QNet(state_dim, action_dim).to(device)
policy_net.load_state_dict(torch.load("pnet.pth"))
target_net = QNet(state_dim, action_dim).to(device)
target_net.load_state_dict(policy_net.state_dict())
max_episodes = 100000
max_steps = 1100
mse = nn.MSELoss()
optim = torch.optim.Adam(policy_net.parameters(), lr=1e-3)
best_score = 0
best_iter_score = 0

def sample(memory, batch_size):
    batch = random.sample(memory, batch_size)
    states, actions, rewards, next_states, dones = zip(*batch)
    states = torch.stack([F.one_hot(torch.tensor(s.board), 4).permute(2,0,1).float() for s in states]).to(device)
    next_states = torch.stack([F.one_hot(torch.tensor(s.board), 4).permute(2,0,1).float() for s in next_states]).to(device)
    actions = torch.tensor(actions, dtype=torch.long).unsqueeze(1).to(device)
    rewards = torch.tensor(rewards, dtype=torch.float32).unsqueeze(1).to(device)
    dones = torch.tensor(dones, dtype=torch.float32).unsqueeze(1).to(device)

    return states, actions, rewards, next_states, dones

def check_move(state, logits):
    if state.canMoveRight == False:
        logits[0] = -torch.inf
    if state.canMoveLeft == False:
        logits[1] = -torch.inf
    if state.canMoveUp == False:
        logits[3] = -torch.inf
    if state.canMoveDown == False:
        logits[2] = -torch.inf
    return logits

# for i in range(max_episodes):
#     print("\nEPISODE: ", i, "Avg Score: ", np.array(scores).mean(), " \n")
#     state = snake.get_state()
#     for j in range(max_steps):
#         epsilon = max(0.05, 1.0 - i / 5000)
#         if random.random() < epsilon:
#             idxs = []
#             if state.canMoveRight:
#                 idxs.append(0)
#             if state.canMoveLeft:
#                 idxs.append(1)
#             if state.canMoveDown:
#                 idxs.append(2)
#             if state.canMoveUp:
#                 idxs.append(3)
#             action = idxs[random.randint(0, len(idxs) - 1)]
#         else:
#             board = torch.tensor(state.board, dtype=torch.long)
#             one_hot = F.one_hot(board, num_classes=4).permute(2,0,1).unsqueeze(0).float().to(device)
#             logits = policy_net(one_hot)
#             logits = logits.squeeze(0)
#             logits = check_move(state, logits)
#             action = torch.argmax(logits).item()
#         obs, rew, done, _ = snake.step(action)

#         mem_buffer.append((state, action, rew, obs, done))
#         if done == True:
#             scores.append(snake.state.score)
#             state = snake.reset()
#             break
#         else:
#             state = obs

#         if j % 32 == 0 and len(mem_buffer) >= 1000:
#             batch_size = 32
#             states, actions, rewards, next_states, dones = sample(mem_buffer, batch_size)
#             q_values = policy_net(states)
#             q_selected = q_values.gather(1, actions)

#             with torch.no_grad():
#                 q_next = target_net(next_states).max(1, keepdim=True)[0]
#                 q_target = torch.where(dones.bool(), rewards, rewards + 0.99 * q_next)

#             loss = mse(q_selected, q_target)

#             optim.zero_grad()
#             loss.backward()
#             optim.step()
#             tau = 0.005
#             for target_param, param in zip(target_net.parameters(), policy_net.parameters()):
#                 target_param.data.copy_(tau * param.data + (1.0 - tau) * target_param.data)
#     if i % 500 == 0 and len(mem_buffer) >= 1000:
#         batch_size = 256
#         states, actions, rewards, next_states, dones = sample(mem_buffer, batch_size)
#         q_values = policy_net(states)
#         q_selected = q_values.gather(1, actions)

#         with torch.no_grad():
#             q_next = target_net(next_states).max(1, keepdim=True)[0]
#             q_target = torch.where(dones.bool(), rewards, rewards + 0.99 * q_next)

#         loss = mse(q_selected, q_target)

#         optim.zero_grad()
#         loss.backward()
#         optim.step()
#         tau = 0.005
#         for target_param, param in zip(target_net.parameters(), policy_net.parameters()):
#             target_param.data.copy_(tau * param.data + (1.0 - tau) * target_param.data)

#     if i % 25 == 0:
#         torch.save(policy_net.state_dict(), "pnet.pth")
#         torch.save(target_net.state_dict(), "target_net.pth")


# torch.save(target_net.state_dict(), "target_net.pth")
# snake.reset()

#render
snake.render_init()
for i in range(max_episodes):
    print("\nEPISODE: ", i, " \n")
    state = snake.get_state()
    for j in range(max_steps):
        sleep(0.05)
        snake.render()
        board = torch.tensor(state.board, dtype=torch.long)
        one_hot = F.one_hot(board, num_classes=4).permute(2,0,1).unsqueeze(0).float().to(device)
        logits = policy_net(one_hot)
        logits = logits.squeeze(0)
        logits = check_move(state, logits)
        action = torch.argmax(logits).item()
        obs, rew, done, _ = snake.step(action)

        mem_buffer.append((state, action, rew, obs, done))
        if done == True:
            state = snake.reset()
            break

snake.render_stop()
