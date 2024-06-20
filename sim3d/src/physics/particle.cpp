#include <iostream>
#include <glm/glm.hpp>
#include "particle.h"

using namespace glm;

particle::particle(glm::vec3 posn, float s, float m) {
    pos = posn;
    size = s;   // radius
    mass = m;
    color = vec3(0.2, 0.1, 0.8);
    velocity = vec3(0);
    oldPos = vec3(0);
}

void particle::setVelocity(glm::vec3 v) {
    velocity = v;
   // oldPos = pos - velocity * 1.0f;
}

void particle::update(float time, glm::vec3 acc) {
    if (velocity != (pos - oldPos) / time)
    {
        oldPos = pos - velocity * time;
    }
    glm::vec3 temp = pos;
    pos += (pos - oldPos) + acc * (time * time);
    oldPos = temp;

    velocity = (pos - oldPos) / time;
}

//copy constructor to detect when copies are created
particle::particle(const particle& other) 
    : pos(other.pos), oldPos(other.oldPos), size(other.size), mass(other.mass), color(other.color), velocity(other.velocity)
{}
