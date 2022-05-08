# Internals

## Repository Layout

The repository layout is pretty standard for a '[zproject](https://github.com/zeromq/zproject)' based project.

- `CMakeLists.txt` -- root CMake file
- `CODE_OF_CONDUCT.md`
- `configure` -- conf script allowing quick access to script utils (sets the env)
- `examples/` -- client - server example code
- `include/` -- .so shard lib headers (interface)
- `libs/` -- helper utilities compiled to shared libs
- `LICENSE.md`
- `README.md`
- `scripts` -- general purpouse utilities
- `sphinx_docs/` -- Sources for the documentation.
- `src/` -- tcp server lib source
