#ifndef SKYNET_TRANSFORMABLE_H
#define SKYNET_TRANSFORMABLE_H

#include "core.h"
#include "data.h"
#include "math.h"
#include <cstring>

class Transformable
{
public:
	void updateRotatation(const RotAxis &type);
	void updateTransform(const RotType &type);

	void setTransform(float *T_) { std::copy(T_, T_ + 16, this->T); }
	float* getWorldTransform() { return T; }

	inline float* getTransform(const RotType &type = RotType::Quaternion) { 
		this->updateTransform(type); return T;
	}

protected:
	float p[3] = {0.0f, 0.0f, 0.0f};
	float e[3] = {0.0f, 0.0f, 0.0f};
    float q[4] = {1.0f, 0.0f, 0.0f, 0.0f};
	float s[3] = {1.0f, 1.0f, 1.0f};
	float w[3] = {1.0f, 0.0f, 0.0f}; 
	float a    = 0.0f;

	float R[16];
	float T[16] = {
		1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
	};

	friend class Scene;
	friend class Interface;
	friend class Serializer;
};

#endif // SKYNET_TRANSFORMABLE_H