#pragma once

#include <bgfx/bgfx.h>
#include <glm/glm.hpp>

struct AssetContext {
	void* _dummy;
};

namespace assets {
	
	// read file
	void* readfile(const char* path, size_t* size = nullptr);
	
	// shader programs
	bgfx::ProgramHandle load_program(const char* path);

	bgfx::TextureHandle load_texture(const char* path, glm::ivec2* dimensions = nullptr);
}

