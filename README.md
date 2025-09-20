mkdir build
cd build
cmake ..
cmake --build .
move .so file to root folder
cd ..
uv venv
uv sync
uv run main.py
