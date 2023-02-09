#include "engine/assets.hpp"

#include <SDL2/SDL.h>
#include <bgfx/bgfx.h>

////////////
// STATIC //
////////////

void makeref_free_string(void* mem, void* userdata) {
	SDL_free(mem);
}

////////////
// PUBLIC //
////////////

namespace assets {

	char* readfile(const char* path, size_t* size) {
		size_t datasize;
		void* data = SDL_LoadFile(path, &datasize);
		
		if (size != nullptr) {
			*size = datasize;
		}

		return static_cast<char*>(data);
	}
	
	// shader programs	
	bgfx::ProgramHandle load_program(const char* path) {

		const char* shader_type = "";
		switch (bgfx::getRendererType()) {
			case bgfx::RendererType::Noop:       shader_type = "noop";  break;
			case bgfx::RendererType::Direct3D9:  shader_type = "dx9";   break;
			case bgfx::RendererType::Direct3D11: shader_type = "dx11";  break;
			case bgfx::RendererType::Direct3D12: shader_type = "dx11";  break;
			case bgfx::RendererType::Agc:        shader_type = "agc";   break;
			case bgfx::RendererType::Gnm:        shader_type = "pssl";  break;
			case bgfx::RendererType::Metal:      shader_type = "metal"; break;
			case bgfx::RendererType::Nvn:        shader_type = "nvn";   break;
			case bgfx::RendererType::OpenGL:     shader_type = "glsl";  break;
			case bgfx::RendererType::OpenGLES:   shader_type = "gles";  break;
			case bgfx::RendererType::Vulkan:     shader_type = "spirv"; break;
			case bgfx::RendererType::WebGPU:     shader_type = "spirv"; break;
			case bgfx::RendererType::Count: break;
		};

		printf("loading program: '%s'\n", path);

		// get path to shader
		char path_vs[256], path_fs[256];
		sprintf(path_vs, "%s/%s/%s", path, shader_type, "vertex.bin");
		sprintf(path_fs, "%s/%s/%s", path, shader_type, "fragment.bin");

		printf("frag: %s\nvert: %s\n", path_fs, path_vs);

		// load shader
		size_t src_vs_size, src_fs_size;
		char* src_vs = assets::readfile(path_vs, &src_vs_size);
		char* src_fs = assets::readfile(path_fs, &src_fs_size);

		bgfx::ShaderHandle shader_vs = bgfx::createShader(
				bgfx::makeRef(src_vs, src_vs_size, makeref_free_string));
		//bgfx::setName(shader_vs, path_vs);
		bgfx::ShaderHandle shader_fs = bgfx::createShader(
				bgfx::makeRef(src_fs, src_fs_size, makeref_free_string));
		//bgfx::setName(shader_fs, path_fs);
		
		return bgfx::createProgram(shader_vs, shader_fs, true);
	}
};

