#pragma once

#include <glm/glm.hpp>

using namespace glm;

struct pointLight {
	vec3 pos;
	vec3 color;
	float intensity;
	float attenuation;

	pointLight() : pos(vec3(0)), color(vec3(1)), intensity(1), attenuation(1.0f) {}
	pointLight(vec3 position, vec3 col, float inten) : pos(position), color(col), intensity(inten), attenuation(1)
	{}
};