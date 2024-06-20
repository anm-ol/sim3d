#pragma once
#include <glm/glm.hpp>
#include <vector>
#include "particle.h"
#include "Edge.h"

using namespace std;
using namespace glm;

class Cloth
{
public:
	Cloth(unsigned int width, unsigned int height, float scale);

	unsigned int width, height;
	float scale;
	void initVertices();
	void initEdges();
	void addEdge(int v1, int v2);
	vector<particle> vertices;

private:
	vector<Edge> edges;
};

