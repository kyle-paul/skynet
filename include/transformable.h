#pragma once

#include "core.h"
#include "data.h"
#include "math.h"

class Transformable
{
public:
	void updateRotatation(const Rot &type);
	void updateTransform();
	void setTransform(float *T) { std::copy(T, T + 16, this->T); }
	float* getTransform() { return T; }

private:
	float p[3] = {0.0f, 0.0f, 3.0f};
	float e[3] = {0.0f, 0.0f, 0.0f};
    float q[4] = {1.0f, 0.0f, 0.0f, 0.0f};
	float s[3] = {1.0f, 1.0f, 1.0f};

	float theta = 0.0f;
	float w[3]  = {1.0f, 2.0f, 0.6f}; 

	float T[16];
	float R[16];

};
