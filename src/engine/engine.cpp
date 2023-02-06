#include "engine/engine.hpp"

#include "engine/iscene.hpp"

namespace engine {
	void set_scene(EngineContext& context, IScene* newScene) {
		if (context.scene) {
			context.scene->destroy();
			delete context.scene;
			context.scene = nullptr;
		}

		if (newScene && newScene->create()) {
			context.scene = newScene;
			context.scene->m_context = &context;
		}
	}
}

