#include "Cloth.h"

Cloth::Cloth(unsigned int _width, unsigned int _height, float _scale)
	: width(_width), height(_height), scale(_scale) {
	initVertices();
	initEdges();
}

void Cloth::initVertices() {

	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			int index = y * width + x;
			vec3 posn = vec3(x * scale, y * scale, 0.0f);
            particle p = particle(posn, 1.0f, 1.0f);
			p.pos = posn;
			p.velocity = vec3(0.0f);

            vertices.insert(vertices.begin() + index, p);
		}
	}
}

void Cloth::initEdges() {
    for (unsigned int y = 0; y < height; ++y) {
        for (unsigned int x = 0; x < width; ++x) {
            unsigned int index = y * width + x;

            if (x < width - 1) {
                // Horizontal structural edge
                addEdge(index, index + 1);
            }
            if (y < height - 1) {
                // Vertical structural edge
                addEdge(index, index + width);
            }
            if (x < width - 1 && y < height - 1) {
                // Shear edges
                addEdge(index, index + width + 1);
                addEdge(index + 1, index + width);
            }
            if (x < width - 2) {
                // Horizontal bending edge
                addEdge(index, index + 2);
            }
            if (y < height - 2) {
                // Vertical bending edge
                addEdge(index, index + 2 * width);
            }
        }
    }
}

void Cloth::addEdge(int v1, int v2) {
    Edge edge;
    edge.vertex1 = v1;
    edge.vertex2 = v2;
    edge.restLength = distance(vertices[v1].pos, vertices[v2].pos);
    edges.push_back(edge);
}
