#ifndef SKYNET_COLLISION_CONTACT_H
#define SKYNET_COLLISION_CONTACT_H

#include "Titan.hpp"
#include "BVHData.h"
#include "Component.h"
#include "entt.hpp"

namespace Skynet
{

	namespace Contact
	{
		struct Contact {
			titan::vec3 point;
			titan::vec3 normal;
			titan::real depth;
		};

		void ComputeContact(PointCloud& cloud, AABB& boxA, AABB& boxB, RigidBody& bodyA, RigidBody& bodyB);

	} // namespace Contact

} // namespace Skynet

#endif // SKYNET_COLLISION_CONTACT_H