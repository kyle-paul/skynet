#include "joint.h"

Joint::Joint(float w[3]) {
    std::copy(w, w + 3, this->w);
}

Joint::~Joint() {}