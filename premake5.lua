PROJECT_DIR = path.getabsolute('.')

-- toolchain
require "lib/premake-emscripten/emscripten"

-- 
workspace "newgame"
	configurations { "debug", "release" }

	platforms {
		"linux64",
		"wasm",
		-- win32, win64, android, macos, ...
	}

	language  "C++"
	location  (path.join(PROJECT_DIR, ""))
	objdir    (path.join(PROJECT_DIR, "bin/"))
	targetdir (path.join(PROJECT_DIR, "bin/"))
	
	project "client"
		kind "WindowedApp"
		
		includedirs {
			path.join(PROJECT_DIR, "src/**.hpp"),
			path.join(PROJECT_DIR, "lib/bgfx/include/"),
			path.join(PROJECT_DIR, "lib/bimg/include/"),
			path.join(PROJECT_DIR, "lib/bx/include/"),
		}

		files {
			path.join(PROJECT_DIR, "src/**.cpp"),
		}
		
		-- Linux Options
		filter { "platforms:linux*" }
			links { "GL", "X11", --[[ "Xrandr" ]] }
			links { "bgfxRelease", "bimgRelease", "bxRelease" }
			libdirs {
				path.join(PROJECT_DIR, "lib/bgfx/.build/linux64_gcc/bin/"),
			}

		-- WebAssembly Options
		filter { "platforms:wasm" }
			toolset "emcc"

			defines { "EMSCRIPTEN=1" }
			buildoptions {
			}
			linkoptions {
				"--shell-file src/platform/wasm/shell.html",
				"-s WASM=1", "-s USE_WEBGL2=1", "-s ALLOW_MEMORY_GROWTH=1",
				"lib/bgfx/.build/wasm/bin/bgfxRelease.bc",
				"lib/bgfx/.build/wasm/bin/bimgRelease.bc",
				"lib/bgfx/.build/wasm/bin/bxRelease.bc",
				"-obin/client.html",
			}


