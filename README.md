# Unnamed Game Engine

A barebones framework for building multimedia C++ applications
targeting Linux, Windows, MacOS and the Web (using emscripten).

**Notice:**
Everything in here is very much work-in-progress! Many things
don't work at all, or only on my machine.


## Building

 1. Clone this repo and `cd` into it.
 2. `premake5 gmake2` to generate the Makefile
 3. `make config=debug_linux64` or `make config=debug_wasm`
 4. Binaries (or HTML/JS/Wasm output) will be generated in `bin/`


### Dependencies

Before Building, `premake5` and `emscripten` need to be installed.

#### SDL2

When targeting Linux builds, `SDL2`, `SDL2_image`, `SDL2_mixer` &
`SDL2_net` libraries are expected to be installed.

When targeting WebAssembly, the emscripten ports are used.

#### BGFX

`BGFX`, `BIMG` & `BX` source files are included and built.

