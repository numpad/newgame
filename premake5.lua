-- https://github.com/jpcy/bgfx-minimal-example/blob/master/premake5.lua

-- variables
PROJECT_DIR = path.getabsolute('.')

-- toolchain
require "lib/premake-modules/emscripten"
require "lib/premake-modules/premake-android-studio/android_studio"
require "lib/premake-modules/shaderc"

-- helpers
function bxCompatIncludeDirs()
	-- TODO: Test for Windows & Mac
	filter "platforms:linux* or platforms:android*"
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
		"android",
		-- win32, win64, osx, ...
	}

	language  "C++"
	cppdialect "C++17"
	-- rtti "Off"
	-- exceptionhandling "Off"
	
	filter "not platforms:android*"
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
	
	filter "platforms:android*"
		

	project "bx"
		filter "not platforms:android*"
			kind "StaticLib"
		filter "platforms:android*"
			kind "SharedLib"
		
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
		filter "not platforms:android*"
			kind "StaticLib"
		filter "platforms:android*"
			kind "SharedLib"

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
		filter "not platforms:android*"
			kind "StaticLib"
		filter "platforms:android*"
			kind "SharedLib"
		
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


	project "client"
		filter "not platforms:android*"
			kind "StaticLib"
		filter "platforms:android*"
			kind "SharedLib"
		
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
			path.join(PROJECT_DIR, "lib/stb/"),
		}

		files {
			path.join(PROJECT_DIR, "src/**.cpp"),
		}

		filter "platforms:linux*"
			links {
				"GL", "X11", -- Xrandr?
				"SDL2", "SDL2_mixer", "SDL2_net",
			}

		filter "platforms:wasm"
			buildoptions { }

			-- TODO: use the same SDL2 headers for every platform
			includedirs {
				path.join(PROJECT_DIR, "lib/sdl2/include/"),
			}

			linkoptions {
				"-s USE_SDL=2 -s USE_SDL_NET=2 -s USE_SDL_MIXER=2",
				"--preload-file res",
				"--shell-file src/_platform/wasm/shell.html",
				"-s WASM=1", "-s USE_WEBGL2=1", "-s ALLOW_MEMORY_GROWTH=1",
				"-obin/wasm/client.html",
			}

		filter "platforms:android*"
			files {
				path.join(PROJECT_DIR, "lib/sdl2/src/main/android/SDL_android_main.c"),
			}

			links {
				"GLESv2",
			}

			-- amk_includes {
			-- 	path.join(PROJECT_DIR, "lib/sdl2/Android.mk"),
			-- }

			-- amk_sharedlinks {
			-- 	"SDL2",
			-- }

