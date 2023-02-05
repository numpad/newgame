# Unnamed Game Engine

A barebones framework for building multimedia C++ applications
targeting Linux, Windows, MacOS and the Web (using emscripten).

**Notice:**
Everything in here is very much work-in-progress! Many things
don't work at all, or only on my machine.


## Building

 1. Clone this repo and `cd` into it.
 2. `premake5 gmake2` to generate the Makefile
 3. `make` or `make config={debug|release}_{linux32|linux64|wasm|win32|win64|...}`
 4. Binaries (or HTML/JS/Wasm output) will be generated in `bin/`


### Dependencies

Before Building, `premake5` and `emscripten` need to be installed.


#### [SDL2](https://www.libsdl.org/)

When targeting Linux, `SDL2`, `SDL2_image`, `SDL2_mixer` &
`SDL2_net` libraries are expected to be installed.

When targeting WebAssembly, the emscripten ports are used.


#### [bgfx](https://github.com/bkaradzic/bgfx)

`bgfx`, `bimg` & `bx` source files are included and built.


#### [glm](https://github.com/g-truc/glm)

`glm` source files are included and built.


#### [EnTT](https://github.com/skypjack/entt)

`entt` source files are included and built.


