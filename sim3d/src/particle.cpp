#include <iostream>
#include <glm/glm.hpp>
#include "particle.h"

int numCopies = 0;

particle::particle(glm::vec3 posn, float s, float m) {
    pos = posn;
    size = s;   // radius
    mass = m;
}

void particle::setVelocity(glm::vec3 v) {
    velocity = v;
    oldPos = pos - velocity * 1.0f;
}

void particle::update(float time, glm::vec3 acc) {
    glm::vec3 temp = pos;
    pos += (pos - oldPos) + acc * (time * time);
    oldPos = temp;

    velocity = (pos - oldPos) / time;
}

//copy constructor to detect when copies are created
particle::particle(const particle& other) 
    : pos(other.pos), oldPos(other.oldPos), size(other.size), mass(other.mass), color(other.color), velocity(other.velocity)
{}
