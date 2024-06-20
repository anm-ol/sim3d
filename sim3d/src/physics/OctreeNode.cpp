#include "OctreeNode.h"

OctreeNode::OctreeNode(vec3 min, vec3 max) {
	minBound = min;
	maxBound = max;
}

void OctreeNode::subdivideNode()
{
	vec3 center = (maxBound + minBound) / 2.0f;

	for (int i = 0; i < 8; i++)
	{
		vec3 newMin = minBound;
		vec3 newMax = center;

		// will undersatnd later
		if (i & 1) newMin.x = center.x; else newMax.x = center.x;
		if (i & 2) newMin.y = center.y; else newMax.y = center.y;
		if (i & 4) newMin.z = center.z; else newMax.z = center.z;

		children[i] = new OctreeNode(newMin, newMax);
	}
}

void OctreeNode::insertParticle(particle& p)
{
	vec3 pos = p.pos;

	// particle is out-of-bounds
	if (pos.x < minBound.x || pos.x > maxBound.x ||
		pos.y < minBound.y || pos.y > maxBound.y ||
		pos.z < minBound.z || pos.x > maxBound.z) return;

	// If the node has children, insert into the appropriate child
	if (children[0] != nullptr)
	{
		for (auto& child : children)
		{
			child->insertParticle(p);
		}
		return;
	}

	// Add particle to this node
	particles.push_back(p);

	// If this node exceeds the particle threshold, subdivide it
	if (particles.size() > MAX_PARTICLE_PER_NODE) 
	{ 
		subdivideNode();
		std::vector<particle> particlesToReassign = particles;

		// Reassign particles to the appropriate children
		particles.clear();
		for (particle p : particlesToReassign) {
			for (auto& child : children) {
				child->insertParticle(p);
			}
		}

	}
}

void OctreeNode::removeParticle(particle& p)
{
	vec3 pos = p.pos;
	// particle is out-of-bounds
	if (pos.x < minBound.x || pos.x > maxBound.x ||
		pos.y < minBound.y || pos.y > maxBound.y ||
		pos.z < minBound.z || pos.x > maxBound.z) return;

	// If the node has children, recursively search for the particle
	if (children[0] != nullptr) {
		for (auto& child : children) {
			child->removeParticle(p);
		}
		return;
	}

	// Remove particle from this node
	auto it = std::find(particles.begin(), particles.end(), p);
	if (it != particles.end()) {
		particles.erase(it);
	}
}

void OctreeNode::update()
{
	for (auto& p : this->particles)
	{
		removeParticle(p);
		insertParticle(p);
	}
}