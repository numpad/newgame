#pragma once

#include <bgfx/bgfx.h>

struct AssetContext {
	void* _dummy;
};

namespace assets {
	
	// read file
	char* readfile(const char* path, size_t* size = nullptr);
	
	// shader programs
	bgfx::ProgramHandle load_program(const char* path);
}

