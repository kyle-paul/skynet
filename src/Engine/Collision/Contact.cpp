#include "Contact.h"
#include "GJKSolver.h"

namespace Skynet
{
	namespace Contact
	{
		titan::real ComputeContactDepth(const AABB& boxA, const AABB& boxB, const RigidBody& bodyA, const RigidBody& bodyB, const titan::vec3& axis)
		{
			float centerA = boxA.GetCenter().dot(axis);
			float centerB = boxB.GetCenter().dot(axis);

			titan::vec3 halfA = boxA.GetHalfExtent();
			titan::vec3 halfB = boxB.GetHalfExtent();

			float r1 = 0.0f, r2 = 0.0f;
			
			for (int i = 0; i < 3; i++)
			{
				titan::vec4 cA = bodyA.R.col(i);
				titan::vec4 cB = bodyB.R.col(i);

				r1 += std::abs(axis * titan::vec3(cA)) * halfA[i];
				r2 += std::abs(axis * titan::vec3(cB)) * halfB[i];
			}

			float overlap = (r1 + r2) - std::abs(centerA - centerB);
			return (overlap > 0) ? overlap : -1.0f;
		}

		bool IsPointInsideOBB(const titan::vec3& point, const AABB& box, const RigidBody& body)
		{
			titan::vec3 center = box.GetCenter();
			titan::vec3 half = box.GetHalfExtent();

			for (int i = 0; i < 3; i++) 
			{
				titan::vec4 orientation = body.R.col(i);
				float distance = (point - center) * titan::vec3(orientation);
				if (std::abs(distance) > half[i]) return false;
			}
			return true;
		}

		list<titan::vec3> GetInterSectionPoints(const AABB& boxA, const AABB& boxB, const RigidBody& bodyA, const RigidBody& bodyB)
		{
			list<titan::vec3> points;

			list<titan::vec3> cornersA = boxA.GetCorners();
			list<titan::vec3> cornersB = boxB.GetCorners();

			for (const titan::vec3 p : cornersA)
				if (IsPointInsideOBB(p, boxB, bodyB))
					points.push_back(p);

			for (const titan::vec3& p : cornersB)
				if (IsPointInsideOBB(p, boxA, bodyA))
					points.push_back(p);
			
			return points;
		}

		void ApplyImpulse(RigidBody& bodyA, RigidBody& bodyB, const Contact& contact)
		{	
			/* Compute relative velocity - gap function */
			titan::vec3 n = contact.normal;
			titan::vec3 rA = contact.point - bodyA.x;
			titan::vec3 rB = contact.point - bodyB.x;

			titan::vec3 vA = bodyA.vel + bodyA.omega ^ rA;
			titan::vec3 vB = bodyB.vel + bodyB.omega ^ rB;
			
			float vrel = n * (rA - rB);

			float epsilon = 0.5f;
			float numerator =  -(1.0f + epsilon) * vrel;

			/* Calculate the denominator in four parts */
			float term1 = 1.0f / bodyA.mass;
			float term2 = 1.0f / bodyB.mass;
			float term3 = n * ((bodyA.Iinv * (rA ^ n)) ^ rA);
			float term4 = n * ((bodyB.Iinv * (rB ^ n)) ^ rB);

			/* Compute the impulse magnitude */
			double j = numerator / (term1 + term2 + term3 + term4);
			titan::vec3 impulse = n * j;

			/* Apply the impulse to the bodies */
			bodyA.P = bodyA.P +  impulse;
			bodyB.P = bodyA.P -  impulse;
			
			bodyA.L = bodyA.L + rA ^ impulse;
			bodyB.L = bodyB.L - rB ^ impulse;

			/* recompute auxiliary variables */
			bodyA.vel = bodyA.P * (1.0f / bodyA.mass);
			bodyB.vel = bodyB.P * (1.0f / bodyB.mass);

			bodyA.omega = bodyA.Iinv * bodyA.L;
			bodyB.omega = bodyB.Iinv * bodyB.L;	
		}

		void SAT(AABB& boxA, AABB& boxB, RigidBody& bodyA, RigidBody& bodyB)
		{
			titan::vec3 axes[15];

			titan::vec3 A_axes[3] =
			{
				titan::vec3(bodyA.R(0,0), bodyA.R(1,0), bodyA.R(2,0)),
				titan::vec3(bodyA.R(0,1), bodyA.R(1,1), bodyA.R(2,1)),
				titan::vec3(bodyA.R(0,2), bodyA.R(1,2), bodyA.R(2,2))
			};

			titan::vec3 B_axes[3] = 
			{
				titan::vec3(bodyB.R(0,0), bodyB.R(1,0), bodyB.R(2,0)),
				titan::vec3(bodyB.R(0,1), bodyB.R(1,1), bodyB.R(2,1)),
				titan::vec3(bodyB.R(0,2), bodyB.R(1,2), bodyB.R(2,2))
			};

			int count = 0;

			/* Axes are face normals */
			for (int i = 0; i < 3; i++) 
			{
		        axes[count++] = A_axes[i];
		        axes[count++] = B_axes[i];
		    }

		    /* Axes are cross product of edges */
		    for (int i=0; i < 3; i++) 
		    {
		    	for (int j=0; j < 3; j++)
		    	{
		    		titan::vec3 axis = A_axes[i].cross(B_axes[j]);
		    		if (axis.length() > 1e-6)
		    		{
		    			axes[count++] = axis.normalized();
		    		}
		    	}
		    }

		    /* Check penetration depth */
		    titan::real minDepth = FLT_MAX;
		    titan::vec3 bestAxis;

		    for (int i=0; i < count; i++)
		    {
		    	titan::vec3 axis = axes[i];
		    	titan::real depth = ComputeContactDepth(boxA, boxB, bodyA, bodyB, axis);
		    	
		    	if (depth < 0) return; // SAT found => no collision

		    	if (depth < minDepth)
		    	{
		    		minDepth = depth;
		    		bestAxis = axis;
		    	}
		    }

		    Contact contact;
		    contact.normal = bestAxis.normalized();
		    contact.depth = minDepth;

		    /* Get a set of intersection points */
		    list<titan::vec3> points = GetInterSectionPoints(boxA, boxB, bodyA, bodyB);

		    if (points.size() > 0)
		    {
		    	titan::vec3 avg;
		        for (const auto& p : points) avg = avg +  p;
		        contact.point = avg * (1.0f / (float)points.size());
		    }

			ApplyImpulse(bodyA, bodyB, contact);
		}

		void ComputeContact(PointCloud& cloud, AABB& boxA, AABB& boxB, RigidBody& bodyA, RigidBody& bodyB)
		{
			cloud.Clear();
			cloud.Add(titan::vec3(0.0f, 0.0f, 0.0f));
			SimpleSolverGJK(cloud, boxA, boxB, bodyA, bodyB);
		}

	} // namespace Contact

} // namespace Skynet