
premake.modules.lua = {}
local m = premake.modules.lua

local p = premake

function postBuildCompileShaders(programName)
	-- TODO: support more platforms
	local path = "res/shader/" .. programName .. "/"

	os.execute("bgfx-shaderc -f " .. path .. "vertex.sc   -o " .. path .. "gles/vertex.bin    --platform asm.js         --type vertex   -i lib/bgfx/src")
	os.execute("bgfx-shaderc -f " .. path .. "fragment.sc -o " .. path .. "gles/fragment.bin  --platform asm.js         --type fragment -i lib/bgfx/src")
	os.execute("bgfx-shaderc -f " .. path .. "vertex.sc   -o " .. path .. "spirv/vertex.bin   --platform linux -p spirv --type vertex   -i lib/bgfx/src")
	os.execute("bgfx-shaderc -f " .. path .. "fragment.sc -o " .. path .. "spirv/fragment.bin --platform linux -p spirv --type fragment -i lib/bgfx/src")
end

newaction {
	trigger = "shaderc",
	description = "Compile shaders.",

	onStart = function()
		-- print("Starting Lua generation")
	end,

	onWorkspace = function(wks)
		-- printf("Generating Lua for workspace '%s'", wks.name)
	end,

	onProject = function(prj)
		-- printf("Generating Lua for project '%s'", prj.name)
	end,

	execute = function()
		postBuildCompileShaders("jelly");
		postBuildCompileShaders("sprite");
	end,

	onEnd = function()
		-- print("Lua generation complete")
	end
}

return m

