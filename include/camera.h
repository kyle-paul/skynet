#pragma once

class Camera
{
public:
	Camera();
	~Camera();

	void updateView();
	void updateProjection();

	inline float* getView() { return T; }
	inline float* getProjection() { return P; }

private:
	float fov    = 45.0f;
    float aspect = 1200.0f / 800.0f;
    float znear  = 0.1f;
    float zfar   = 100.0f;

    float p[3]   = {0.0f, 0.0f, 3.0f};
    float e[3]   = {0.0f, 0.0f, 0.0f};
    float q[4]   = {1.0f, 0.0f, 0.0f, 0.0f};

    float ox[3]  = {1.0f, 0.0f, 0.0f};
    float oy[3]  = {0.0f, 1.0f, 0.0f};
    float oz[3]  = {0.0f, 0.0f, 1.0f};

    float T[16];
    float P[16];
};