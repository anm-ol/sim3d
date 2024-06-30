#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <random>
#include <algorithm>

#include "particle.h"

class Engine;

using namespace glm;
struct partition
{
	vec3 minBound;
	vec3 maxBound;
	vec3 color; 

	partition(vec3 min, vec3 max) : minBound(min), maxBound(max)
	{
		color = randomVec3(vec3(0), vec3(1));
	}
	std::vector<particle*> group;

	void addParticle(particle* p);
	vec3 randomVec3(vec3 min, vec3 max)
	{
		std::random_device rd;
		std::mt19937 gen(rd());

		vec3 posn(0.0);
		for (int i = 0; i < posn.length(); i++)
		{
			// Create a uniform distribution for floats within the range [min, max]
			std::uniform_real_distribution<float> dist(min[i], max[i]);
			posn[i] = dist(gen);
		}
		return posn;
	}
};

class spacePartition
{
public:
	std::vector<partition> partitions;
	int subDivs;
	vec3 worldmin, worldmax;
	vec3 worldsize = worldmax - worldmin;
	vec3 boxsize;
	Engine& engine;

	spacePartition(Engine& engine, int divisions);
	

		void createGrid(vec3 min, vec3 max);
		void setParticles(std::vector<particle>&particles);
		void partitionCollide();
		void reset();
		int getCellIndex(vec3 pos);
};