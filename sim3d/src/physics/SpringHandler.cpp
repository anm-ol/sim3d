#include "SpringHandler.h"
#include "Engine.h"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp>
#include <glm/gtx/euler_angles.hpp>
using namespace glm;

// width and height refers to num particles in x and y axes respectively
SpringHandler::SpringHandler(std::vector<particle> *particles, int x, int y, float s, float m)
	: targetVector(particles), num_x(x), num_y(y), size(s), mass(m) 
{
    isInit = true;
};

void SpringHandler::init(Engine& engine)
{
    //initVertices(engine);
    //initSprings();
}

// adding particles as vertices
void SpringHandler::initVertices(Engine& engine, vec3 startPos, float spacing) {
    unsigned int total_num = num_x * num_y;
    m_spacing = spacing;
    m_startPos = startPos;
    center = total_num / 2 + num_x /2;
	particleIDs.reserve(total_num);
    targetVector->reserve(total_num);

	for (int y = 0; y < num_y; y++) {
		for (int x = 0; x < num_x; x++) {
			vec3 posn =  startPos + vec3(x * (size * spacing), y * (size * spacing), 0.0f);
            targetVector->emplace_back(posn, size, mass);
            targetVector->back().setVelocity(randomVec3(vec3(-0.01), vec3(0.01)));
            particleIDs.emplace_back(targetVector->size() - 1);
            particlePositions.push_back(posn);
		}
	}
}

void SpringHandler::addSpring(particle& p1, particle& p2)
{
    
}

void SpringHandler::initSprings() {
    springs.reserve(num_x * num_y * 6);
    for (int y = 0; y < num_y; y++) {
        for (int x = 0; x < num_x; x++) {
            int index = y * num_x + x;
            unsigned int ID = particleIDs[index];

            if (x < num_x - 1) {
                // Horizontal structural edge
                springs.emplace_back((*targetVector)[ID], (*targetVector)[particleIDs[index + 1]], structCoeff);
            }
            if (y < num_y - 1) {
                // Vertical structural edge
                springs.emplace_back((*targetVector)[ID], (*targetVector)[particleIDs[index + num_x]], structCoeff);
            }
            if (x < num_x - 1 && y < num_y - 1) {
                // Shear edges
                springs.emplace_back((*targetVector)[ID], (*targetVector)[particleIDs[index + num_x + 1]], shearCoeff);
                springs.emplace_back((*targetVector)[particleIDs[index + 1]], (*targetVector)[particleIDs[index + num_x]], shearCoeff);
            }
            if (x < num_x - 2) {
                // Horizontal bending edge
                springs.emplace_back((*targetVector)[ID], (*targetVector)[particleIDs[index + 2]], bendingCoeff);
            }
            if (y < num_y - 2) {
                // Vertical bending edge
                springs.emplace_back((*targetVector)[ID], (*targetVector)[particleIDs[index + 2 * num_x]], bendingCoeff);
            }
        }
    }
}

void SpringHandler::updateForce()
{
    //reset force to zero 
    for (int i : particleIDs)
    {
        auto ID = particleIDs[i];
        auto& p = (*targetVector)[ID];
        particlePositions[i] = p.pos;
        p.force = vec3(0);
    }

    //re-calculate force with updated vertex positions
    for (spring& spr : springs)
    {
        spr.setForce();
    }
}


// used for translating the entire cloth
void SpringHandler::translate(vec3 translate) {
    for (int ID : particleIDs) {
        auto& p = (*targetVector)[ID];
        
        p.pos += translate;
        particlePositions[ID] = p.pos;
    }
}

void SpringHandler::rotatecloth(vec3 rotation) {
    rotation = radians(rotation);
    for (auto ID : particleIDs) {
        auto& p = (*targetVector)[ID];
        vec3 centerPos = particlePositions[center];
        glm::mat4 rotationMatrix = glm::yawPitchRoll(rotation.y, rotation.x, rotation.z);
        vec3 rotatedPos = vec3(rotationMatrix * vec4(p.pos - centerPos, 0.0f));
        p.pos = centerPos + rotatedPos;
        particlePositions[ID] = p.pos;
    }
}