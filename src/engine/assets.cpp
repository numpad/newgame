#include "engine/assets.hpp"

#include <SDL2/SDL.h>
#include <bgfx/bgfx.h>
#include <bimg/decode.h>
#include <glm/glm.hpp>
#include <stb_image.h>

////////////
// STATIC //
////////////

static void makeref_free_string(void* mem, void* userdata) {
	SDL_free(mem);
}

static void makeref_free_image(void* mem, void* userdata) {
	bimg::ImageContainer* container = static_cast<bimg::ImageContainer*>(userdata);
	bimg::imageFree(container);
}

////////////
// PUBLIC //
////////////

namespace assets {

	void* readfile(const char* path, size_t* size) {
		size_t datasize;
		void* data = SDL_LoadFile(path, &datasize);
		
		if (size != nullptr) {
			*size = datasize;
		}

		return data;
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

		// get path to shader
		char path_vs[256], path_fs[256];
		sprintf(path_vs, "%s/%s/%s", path, shader_type, "vertex.bin");
		sprintf(path_fs, "%s/%s/%s", path, shader_type, "fragment.bin");

		// load shader
		size_t src_vs_size, src_fs_size;
		char* src_vs = static_cast<char*>(assets::readfile(path_vs, &src_vs_size));
		char* src_fs = static_cast<char*>(assets::readfile(path_fs, &src_fs_size));

		bgfx::ShaderHandle shader_vs = bgfx::createShader(bgfx::makeRef(src_vs, src_vs_size, makeref_free_string));
		bgfx::ShaderHandle shader_fs = bgfx::createShader(bgfx::makeRef(src_fs, src_fs_size, makeref_free_string));
		
		return bgfx::createProgram(shader_vs, shader_fs, true);
	}

	bgfx::TextureHandle load_texture(const char* path, glm::ivec2* dimensions) {
		glm::ivec2 size;
		int channels;
		stbi_set_flip_vertically_on_load(false);

		uint8_t* data = stbi_load(path, &size.x, &size.y, &channels, 0);
		
		const uint32_t flags = BGFX_SAMPLER_U_CLAMP | BGFX_SAMPLER_V_CLAMP | BGFX_SAMPLER_MIN_POINT | BGFX_SAMPLER_MAG_POINT;
		bgfx::TextureHandle handle = bgfx::createTexture2D(size.x, size.y, false, 1, bgfx::TextureFormat::RGBA8, flags, bgfx::copy(data, size.x * size.y * channels));

		if (!bgfx::isValid(handle)) {
			printf("failed loading texture '%s'\n", path);
			return BGFX_INVALID_HANDLE;
		}
		
		if (dimensions != nullptr) {
			dimensions->x = size.x;
			dimensions->y = size.y;
		}

		stbi_image_free(data);
		return handle;
	}
};

