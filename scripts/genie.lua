PROJECT_DIR = path.getabsolute('..')

-- toolchain

-- 
solution "newgame"
	configurations {
		"Debug",
		"Release",
	}

	platforms {
		"x64",
		"x32",
		"native",
	}

	language  "C++"
	location  (path.join(PROJECT_DIR, ""))
	objdir    (path.join(PROJECT_DIR, "bin/"))
	targetdir (path.join(PROJECT_DIR, "bin/"))
	
	project "client"
		kind "WindowedApp"
		links { "bgfxRelease", "bimgRelease", "bxRelease" }
		
		includedirs {
			path.join(PROJECT_DIR, "src/**.hpp"),
			path.join(PROJECT_DIR, "lib/bgfx/include/"),
			path.join(PROJECT_DIR, "lib/bimg/include/"),
			path.join(PROJECT_DIR, "lib/bx/include/"),
		}

		files {
			path.join(PROJECT_DIR, "src/**.cpp"),
		}

		configuration { "linux-*" }
			links { "GL", "X11", --[[ "Xrandr" ]] }
			libdirs {
				path.join(PROJECT_DIR, "lib/bgfx/.build/linux64_gcc/bin/"),
			}


		configuration {}

