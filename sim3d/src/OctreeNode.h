#pragma once
#include <glm/glm.hpp>
#include <iostream>
#include <vector>
#include "particle.h"

using namespace glm;
using namespace std;

class OctreeNode
{
private:
	vec3 minBound, maxBound;
	vector<particle> particles;	// Particles contained in this node	
	OctreeNode* children[8];	// Pointers to child nodes

public:
	const int MAX_PARTICLE_PER_NODE = 8;

	OctreeNode(vec3 minBound, vec3 maxBound);

	void subdivideNode();
	void insertParticle(particle& particle);
	void removeParticle(particle& particle);
	void update();
};