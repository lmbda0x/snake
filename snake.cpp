#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <tuple>
extern "C" {
#include "snake.h"
}

struct State {
  std::array<std::array<int, 12>, 12> board;
  bool canMoveRight;
  bool canMoveLeft;
  bool canMoveUp;
  bool canMoveDown;
};

namespace py = pybind11;

State get_game_state(GameState *state) {
  std::array<std::array<int, 12>, 12> board{};
  SnakePart *temp = state->root;
  int offset = 1;
  while (temp != NULL) {
    int ix1 = ((int)temp->pos.x / 40) + offset;
    int ix2 = ((int)temp->pos.y / 40) + offset;
    if (temp->next == NULL) {
      board[ix1][ix2] = 2;
      temp = temp->next;
      continue;
    }
    board[ix1][ix2] = 1;
    temp = temp->next;
  }
  int ix1 = ((int)state->fruitPosition.x / 40) + offset;
  int ix2 = ((int)state->fruitPosition.y / 40) + offset;
  board[ix1][ix2] = 3;
  bool canMoveRight = state->dir != 1;
  bool canMoveLeft = state->dir != 0;
  bool canMoveUp = state->dir != 2;
  bool canMoveDown = state->dir != 3;
  return State{board, canMoveRight, canMoveLeft, canMoveUp, canMoveDown};
}

std::tuple<State, double, bool> do_game_step(GameState *state, int action) {
  GameState prev = *state;
  game_step(state, action);
  State obs = get_game_state(state);
  bool done = state->gameOver == 1;
  double reward = 0.0;
  if (done == true) {
    reward = -100.0;
  } else if ((prev.dir == 0 && state->dir == 1) ||
             (prev.dir == 1 && state->dir == 0) ||
             (prev.dir == 3 && state->dir == 2) ||
             (prev.dir == 2 && state->dir == 3)) {
    reward = -100.0;
  } else if (prev.score < state->score) {
    reward = 100.0;
  }

  return std::make_tuple(obs, reward, done);
}

PYBIND11_MODULE(snake_module, m) {
  py::class_<Vector2>(m, "Vector2")
      .def(py::init<>())
      .def_readwrite("x", &Vector2::x)
      .def_readwrite("y", &Vector2::y)
      .def("__repr__", [](const Vector2 &v) {
        return py::str("Vector2(x={}, y={})").format(v.x, v.y);
      });

  py::class_<GameState>(m, "GameState")
      .def(py::init<>())
      .def_readwrite("width", &GameState::width)
      .def_readwrite("height", &GameState::height)
      .def_readwrite("dir", &GameState::dir)
      .def_readwrite("score", &GameState::score)
      .def_readwrite("gameOver", &GameState::gameOver)
      .def_readwrite("root", &GameState::root)
      .def_readwrite("fruitPosition", &GameState::fruitPosition);

  py::class_<State>(m, "State")
      .def(py::init<>())
      .def_readonly("board", &State::board)
      .def_readonly("canMoveRight", &State::canMoveRight)
      .def_readonly("canMoveLeft", &State::canMoveLeft)
      .def_readonly("canMoveUp", &State::canMoveUp)
      .def_readonly("canMoveDown", &State::canMoveDown)
      .def("__repr__", [](const State &s) {
        return py::str("State(board={}, right?={}, left?={}, up?={}, down?={})")
            .format(s.board, s.canMoveRight, s.canMoveLeft, s.canMoveUp,
                    s.canMoveDown);
      });

  py::class_<SnakePart>(m, "SnakePart")
      .def(py::init<>())
      .def_readwrite("pos", &SnakePart::pos)
      .def_readwrite("next", &SnakePart::next);

  m.def("game_init", &game_init, "Initialize the game state", py::arg("state"));
  m.def("game_step", &game_step, "Perform a game step with an action",
        py::arg("state"), py::arg("action"));
  m.def("do_game_step", &do_game_step, "Do a game step", py::arg("state"),
        py::arg("action"));
  m.def("get_game_state", &get_game_state, "Get current game state for the NN",
        py::arg("state"));
  m.def("game_reset", &game_reset, "Reset the game state", py::arg("state"));
  m.def("game_render", &game_render, "Render the game state", py::arg("state"));
  m.def("game_clear", &game_clear, "Clear the game state", py::arg("state"));
  m.def("createNode", &createNode, "Create a new SnakePart node");
  m.def("render_init", &render_init, "Initialize rendering");
  m.def("render_stop", &render_stop, "Stop rendering");
}
