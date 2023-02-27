-- https://github.com/jpcy/bgfx-minimal-example/blob/master/premake5.lua

-- variables
PROJECT_DIR = path.getabsolute('.')

-- toolchain
require "lib/premake-modules/premake-android-studio/android_studio"
require "lib/premake-modules/shaderc"

-- helpers
function bxCompatIncludeDirs()
	includedirs { path.join(PROJECT_DIR, "lib/bx/include/compat/linux/") }
end

workspace "newgame"
	configurations { "Debug", "Release" }

	language  "C++"
	cppdialect "C++17"
	-- rtti "Off"
	-- exceptionhandling "Off"
	
	location  (path.join(PROJECT_DIR, ""))
	-- objdir    (path.join(PROJECT_DIR, "bin/"))
	-- targetdir (path.join(PROJECT_DIR, "bin/"))
	
	androidabis { "armeabi-v7a", "arm64-v8a", "x86", "x86_64" }

	gradleversion "com.android.tools.build:gradle:7.3.1"
	androidndkpath "/home/chris/.config/android-studio/ndk-bundle/" -- TODO: fix this
	androiddependencies {
		"com.android.support:support-v4:27.1.0",
	}

	-- gradleproperties {
	-- 	"org.gradle.jvmargs=-Xmx4608m",
	-- 	"org.gradle.parallel=true",
	-- }
	gradlewrapper {
		"distributionUrl=https://services.gradle.org/distributions/gradle-7.5.1-bin.zip"
	}
	androidrepositories {
		-- "jcenter()",
		-- "maven { url 'http://maven.gameanalytics.com/release' }",
	}

	filter "configurations:debug"
		defines { "BX_CONFIG_DEBUG=1" }
		optimize "Debug"
		symbols "On"

	filter "configurations:release"
		defines { "BX_CONFIG_DEBUG=0", "NDEBUG" }
		optimize "Full"
	

	project "bx"
		kind "SharedLib"
		
		androidsdkversion "28"
		androidminsdkversion "28"

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
		kind "SharedLib"
		
		androidsdkversion "28"
		androidminsdkversion "28"

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
		kind "SharedLib"
		
		androidsdkversion "28"
		androidminsdkversion "28"
		
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
		kind "WindowedApp"
		
		androidsdkversion "28"
		androidminsdkversion "28"
		
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
			path.join(PROJECT_DIR, "lib/sdl2/include/"),
		}

		files {
			path.join(PROJECT_DIR, "src/**.cpp"),
			path.join(PROJECT_DIR, "lib/sdl2/src/main/android/SDL_android_main.c"),
			path.join(PROJECT_DIR, "src/_platform/android/main_activity.java"),
		}

		links { "GLESv2", }

		assetdirs { "res" }

