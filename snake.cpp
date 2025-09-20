#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
extern "C" {
#include "snake.h"
}

namespace py = pybind11;

PYBIND11_MODULE(snake_module, m) {
  py::class_<Vector2>(m, "Vector2")
      .def(py::init<>())
      .def_readwrite("x", &Vector2::x)
      .def_readwrite("y", &Vector2::x)
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

  py::class_<SnakePart>(m, "SnakePart")
      .def(py::init<>())
      .def_readwrite("pos", &SnakePart::pos)
      .def_readwrite("next", &SnakePart::next);

  m.def("game_init", &game_init, "Initialize the game state", py::arg("state"));
  m.def("game_step", &game_step, "Perform a game step with an action",
        py::arg("state"), py::arg("action"));
  m.def("game_reset", &game_reset, "Reset the game state", py::arg("state"));
  m.def("game_render", &game_render, "Render the game state", py::arg("state"));
  m.def("game_clear", &game_clear, "Clear the game state", py::arg("state"));
  m.def("createNode", &createNode, "Create a new SnakePart node");
}
