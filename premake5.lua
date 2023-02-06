-- https://github.com/jpcy/bgfx-minimal-example/blob/master/premake5.lua

-- variables
PROJECT_DIR = path.getabsolute('.')

-- toolchain
require "lib/premake-modules/emscripten"

-- helpers
function bxCompatIncludeDirs()
	-- TODO: Test for Windows & Mac
	filter "platforms:linux*"
		includedirs { path.join(PROJECT_DIR, "lib/bx/include/compat/linux/") }
	filter "platforms:win*"
		includedirs { path.join(PROJECT_DIR, "lib/bx/include/compat/msvc/") }
	filter "platforms:osx*"
		includedirs { path.join(PROJECT_DIR, "lib/bx/include/compat/osx/") }
end

workspace "newgame"
	configurations { "debug", "release" }

	platforms {
		"linux64",
		"wasm",
		-- win32, win64, android, osx, ...
	}

	language  "C++"
	cppdialect "C++17"
	-- rtti "Off"
	-- exceptionhandling "Off"
	
	location  (path.join(PROJECT_DIR, ""))
	-- objdir    (path.join(PROJECT_DIR, "bin/"))
	-- targetdir (path.join(PROJECT_DIR, "bin/"))
	
	filter "configurations:debug"
		defines { "BX_CONFIG_DEBUG=1" }
		optimize "Debug"
		symbols "On"

	filter "configurations:release"
		defines { "BX_CONFIG_DEBUG=0", "NDEBUG" }
		optimize "Full"
	
	filter "platforms:wasm"
		toolset "emcc"

		defines {
			"EMSCRIPTEN=1",
			"BGFX_CONFIG_RENDERER_OPENGLES=20",
		}
	

	project "bx"
		kind "StaticLib"
		
		files {
			path.join(PROJECT_DIR, "lib/bx/include/bx/*.h"),
			path.join(PROJECT_DIR, "lib/bx/include/bx/inline/*.inl"),
			path.join(PROJECT_DIR, "lib/bx/src/*.cpp"),
		}

		excludes {
			path.join(PROJECT_DIR, "lib/bx/src/amalgamated.cpp"),
			path.join(PROJECT_DIR, "lib/bx/src/crtnone.cpp"),
		}

		includedirs {
			path.join(PROJECT_DIR, "lib/bx/3rdparty/"),
			path.join(PROJECT_DIR, "lib/bx/include/"),
		}

		bxCompatIncludeDirs()


	project "bimg"
		kind "StaticLib"

		files {
			path.join(PROJECT_DIR, "lib/bimg/include/bimg/*.h"),
			path.join(PROJECT_DIR, "lib/bimg/src/image.cpp"),
			path.join(PROJECT_DIR, "lib/bimg/src/image_gnf.cpp"),
			path.join(PROJECT_DIR, "lib/bimg/src/*.h"),
			path.join(PROJECT_DIR, "lib/bimg/3rdparty/astc-encoder/source/*.cpp"),
		}

		includedirs {
			path.join(PROJECT_DIR, "lib/bx/include/"),
			path.join(PROJECT_DIR, "lib/bimg/include/"),
			path.join(PROJECT_DIR, "lib/bimg/3rdparty/astc-encoder/"),
			path.join(PROJECT_DIR, "lib/bimg/3rdparty/astc-encoder/include/"),
		}

		bxCompatIncludeDirs()
	

	project "bgfx"
		kind "StaticLib"
		
		files {
			path.join(PROJECT_DIR, "lib/bgfx/include/bgfx/*.h"),
			path.join(PROJECT_DIR, "lib/bgfx/src/*.cpp"),
			path.join(PROJECT_DIR, "lib/bgfx/src/*.h"),
		}

		excludes {
			path.join(PROJECT_DIR, "lib/bgfx/src/amalgamated.cpp"),
		}

		includedirs {
			path.join(PROJECT_DIR, "lib/bx/include/"),
			path.join(PROJECT_DIR, "lib/bimg/include/"),
			path.join(PROJECT_DIR, "lib/bgfx/include/"),
			path.join(PROJECT_DIR, "lib/bgfx/3rdparty/"),
			path.join(PROJECT_DIR, "lib/bgfx/3rdparty/directx-headers/include/"),
			path.join(PROJECT_DIR, "lib/bgfx/3rdparty/directx-headers/include/directx/"),
			path.join(PROJECT_DIR, "lib/bgfx/3rdparty/directx-headers/include/wsl/stubs/"),
			path.join(PROJECT_DIR, "lib/bgfx/3rdparty/khronos/"),
		}

		bxCompatIncludeDirs()
	
	--[[
	project "sdl2"
		kind "SharedLib"
		
		files {
			path.join(PROJECT_DIR, "lib/sdl2/src/**.c"),
		}
		excludes {
			path.join(PROJECT_DIR, "lib/sdl2/src/core/linux/SDL_fcitx.*"),
		}

		includedirs {
			path.join(PROJECT_DIR, "lib/sdl2/include/"),
			path.join(PROJECT_DIR, "lib/sdl2/src/**.h"),
		}
	]]
	
	project "client"
		kind "WindowedApp"
		links { "bgfx", "bimg", "bx" }

		libdirs {
			path.join(PROJECT_DIR, "lib/bin/"),
		}

		includedirs {
			path.join(PROJECT_DIR, "src/"),
			path.join(PROJECT_DIR, "lib/bgfx/include/"),
			path.join(PROJECT_DIR, "lib/bimg/include/"),
			path.join(PROJECT_DIR, "lib/bx/include/"),
			path.join(PROJECT_DIR, "lib/glm/include/"),
			path.join(PROJECT_DIR, "lib/entt/single_include/"),
		}

		files {
			path.join(PROJECT_DIR, "src/**.cpp"),
		}
		
		filter "platforms:linux*"
			links {
				"GL", "X11", -- Xrandr?
				"SDL2", "SDL2_image", "SDL2_mixer", "SDL2_net",
			}

		filter "platforms:wasm"
			buildoptions { }

			linkoptions {
				"-s USE_SDL=2 -s USE_SDL_IMAGE=2 -s SDL2_IMAGE_FORMATS='[\"png\"]'",
				"-s USE_SDL_NET=2 -s USE_SDL_MIXER=2",
				"--preload-file res",
				"--shell-file src/_platform/wasm/shell.html",
				"-s WASM=1", "-s USE_WEBGL2=1", "-s ALLOW_MEMORY_GROWTH=1",
				"-obin/wasm/client.html",
			}

