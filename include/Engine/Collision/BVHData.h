#ifndef SKYNET_COLLISION_BVHDATA_H
#define SKYNET_COLLISION_BVHDATA_H

#include "RigidBody.h"
#include "Mesh.h"

namespace Skynet
{
    struct OBB {
        titan::vec3 center;
        titan::mat3 rotation;
        titan::vec3 halfExtents;

        OBB() = default;
        ~OBB() = default;

        list<titan::vec3> GetCorners() const {
            list<titan::vec3> corners(8);

            titan::vec3 xAxis = rotation.col(0) * halfExtents[0];
            titan::vec3 yAxis = rotation.col(1) * halfExtents[1];
            titan::vec3 zAxis = rotation.col(2) * halfExtents[2];

            corners[0] = center + xAxis + yAxis + zAxis;
            corners[1] = center - xAxis + yAxis + zAxis;
            corners[2] = center + xAxis - yAxis + zAxis;
            corners[3] = center - xAxis - yAxis + zAxis;
            corners[4] = center + xAxis + yAxis - zAxis;
            corners[5] = center - xAxis + yAxis - zAxis;
            corners[6] = center + xAxis - yAxis - zAxis;
            corners[7] = center - xAxis - yAxis - zAxis;

            return corners;
        }

        bool Intersect(const OBB* other) const {
            return false;
        }
    };


    struct AABB {
        titan::vec3 min = titan::vec3(FLT_MAX, FLT_MAX, FLT_MAX);
        titan::vec3 max = titan::vec3(-FLT_MAX, -FLT_MAX, -FLT_MAX);

        AABB() = default;
        ~AABB() = default;

        inline list<titan::vec3> GetCorners() const {
            list<titan::vec3> corners(8);
            corners[0] = {min[0], min[1], min[2]};
            corners[1] = {max[0], min[1], min[2]};
            corners[2] = {min[0], max[1], min[2]};
            corners[3] = {max[0], max[1], min[2]};
            corners[4] = {min[0], min[1], max[2]};
            corners[5] = {max[0], min[1], max[2]};
            corners[6] = {min[0], max[1], max[2]};
            corners[7] = {max[0], max[1], max[2]};
            return corners;
        }

        inline titan::vec3 GetTransMax(const titan::mat4& T) const {
            return T * this->max;
        }

        inline titan::vec3 GetTransMin(const titan::mat4& T) const {
            return T * this->min;
        }

        inline titan::vec3 GetCenter() const {
            return (this->min + this->max) * 0.5f;
        }

        inline titan::vec3 GetHalfExtent() const {
            return (this->max - this->min) * 0.5;
        }

        inline titan::vec3 GetSize() const {
            return titan::vec3(max[0] - min[0], max[1] - min[1], max[2] - min[2]);
        }

        void Expand(titan::vec3 point) {
            this->min = titan::MinVec3(min, point);
            this->max = titan::MaxVec3(max, point);
        }

        void Expand(const Triangle& tri) {
            this->Expand(tri.points[0]);
            this->Expand(tri.points[1]);
            this->Expand(tri.points[2]);
        }

        void Expand(AABB& other) {
            this->min = titan::MinVec3(min, other.min);
            this->max = titan::MaxVec3(max, other.max);
        }
    };
    

    struct BVHNode {
        AABB box;
        list<uint32_t> triangles;
        BVHNode* childA = nullptr;
        BVHNode* childB = nullptr;

        BVHNode() = default;
        ~BVHNode() {
            if (childA) delete childA;
            if (childB) delete childB;
        }

        inline bool IsLeaf() {
            return (!childA && !childB);
        }
    };

} // namespace Skynet


#endif // SKYNET_COLLISION_BVHDATA_H