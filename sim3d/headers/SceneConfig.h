#pragma once
#include "Engine.h"
#include <glm/glm.hpp>

using namespace glm;

class SceneConfig {

public:
	vec3 min, max;


	SceneConfig();

	void start();
	void reset();
	void pause();
};