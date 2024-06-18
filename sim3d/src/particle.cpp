#include <iostream>
#include <glm/glm.hpp>
#include "particle.h"

int numCopies = 0;

particle::particle(glm::vec3 posn, float s, float m) {
    pos = posn;
    size = s;   // radius
    mass = m;
    velocity = glm::vec3(0);
}

void particle::setVelocity(glm::vec3 v) {
    velocity = v;
}

glm::vec3 particle::getPosition() {
    return pos;
}

void particle::update(float time) {
    pos += velocity * time;
}

//copy constructor to detect when copies are created
particle::particle(const particle& other) : pos(other.pos), velocity(other.velocity), size(other.size), mass(other.mass)
{
   // std::cout << "numcopies " << numCopies++ << std::endl;
}
