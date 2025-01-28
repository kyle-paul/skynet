#pragma once

class Camera
{
public:
	Camera();
	~Camera();

	void updateView();
    void updateProjection();
	void updateProjView();

	inline float* getView() { return V; }
	inline float* getProjection() { return P; }
    inline float* getProjView() { return C; }

public:
	float fov    = 45.0f;
    float aspect = 1500.0f / 900.0f;
    float znear  = 0.1f;
    float zfar   = 100.0f;

    float p[3]   = {0.0f, 0.0f, 3.0f};
    float e[3]   = {0.0f, 0.0f, 0.0f};
    float q[4]   = {1.0f, 0.0f, 0.0f, 0.0f};

    float ox[3]  = {1.0f, 0.0f, 0.0f};
    float oy[3]  = {0.0f, 1.0f, 0.0f};
    float oz[3]  = {0.0f, 0.0f, 1.0f};

    float V[16];
    float P[16];
    float C[16];
};