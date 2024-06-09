#include <iostream>
#include <glm/glm.hpp>
#include "particle.h"

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

bool particle::operator==(const particle& other) const
{
    return pos == other.pos;
}
