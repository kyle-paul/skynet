#ifndef SKYNET_ENGINE_H
#define SKYNET_ENGINE_H

#include "core.h"
#include "link.h"
#include "math.h"

namespace engine {

inline void jacobian(float* J, float* old_e, float* cur_e, std::unordered_map<std::string, ref<Joint>> &joints) {
    size_t n = 0;
    size_t m = joints.size();

    for (auto &[name, joint] : joints) {
        J[n]     = (cur_e[0] - old_e[0]) / DEG2RAD(joint->delta_a);
        J[n+m]   = (cur_e[1] - old_e[1]) / DEG2RAD(joint->delta_a);
        J[n+2*m] = (cur_e[2] - old_e[2]) / DEG2RAD(joint->delta_a);
        n++;
    }   

    math::printMat(J, n, m);
    math::pseudoinvert(J, n, m);
}

}

#endif // SKYNET_ENGINE_H