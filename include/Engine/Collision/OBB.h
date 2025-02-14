#ifndef SKYNET_COLLISION_OBB_H
#define SKYNET_COLLISION_OBB_H

#include "BVH.h"
#include "Math.h"

namespace Skynet
{
    struct Mat3x3 
    {
        float m[3][3];
        Mat3x3() {
            for (int i = 0; i < 3; ++i)
                for (int j = 0; j < 3; ++j)
                    m[i][j] = 0.0f;
        }

        float& operator()(int row, int col) { return m[row][col]; }
        const float& operator()(int row, int col) const { return m[row][col]; }
    };

    // Jacobi Eigenvalue Algorithm
    void JacobiEigenvalues(Mat3x3& A, float eigenvalues[3], float eigenvectors[3][3]) {
        const int maxIter = 50;
        const float epsilon = 1e-6f;

        for (int i = 0; i < 3; ++i) 
        {
            eigenvalues[i] = A(i, i);
            for (int j = 0; j < 3; ++j)
                eigenvectors[i][j] = (i == j) ? 1.0f : 0.0f;
        }

        for (int iter = 0; iter < maxIter; ++iter) 
        {
            int p = 0, q = 1;
            float maxVal = std::fabs(A(0, 1));

            if (std::fabs(A(0, 2)) > maxVal) { p = 0; q = 2; maxVal = std::fabs(A(0, 2)); }
            if (std::fabs(A(1, 2)) > maxVal) { p = 1; q = 2; maxVal = std::fabs(A(1, 2)); }

            if (maxVal < epsilon) break;

            float theta = 0.5f * std::atan2(2.0f * A(p, q), A(q, q) - A(p, p));
            float c = std::cos(theta);
            float s = std::sin(theta);

            float app = c * c * A(p, p) - 2.0f * s * c * A(p, q) + s * s * A(q, q);
            float aqq = s * s * A(p, p) + 2.0f * s * c * A(p, q) + c * c * A(q, q);
            float apq = 0.0f;

            A(p, p) = app;
            A(q, q) = aqq;
            A(p, q) = A(q, p) = apq;

            for (int k = 0; k < 3; ++k) {
                if (k != p && k != q) {
                    float akp = c * A(k, p) - s * A(k, q);
                    float akq = s * A(k, p) + c * A(k, q);
                    A(k, p) = A(p, k) = akp;
                    A(k, q) = A(q, k) = akq;
                }
            }

            for (int k = 0; k < 3; ++k) {
                float evkp = c * eigenvectors[k][p] - s * eigenvectors[k][q];
                float evkq = s * eigenvectors[k][p] + c * eigenvectors[k][q];
                eigenvectors[k][p] = evkp;
                eigenvectors[k][q] = evkq;
            }
        }
    }

    namespace BVH
    {
        void FitNormal(float* verts, float& n, BoundingBox* box)
        {
            float min_point[3] = { std::numeric_limits<float>::max(), 
                                   std::numeric_limits<float>::max(), 
                                   std::numeric_limits<float>::max() };

            float max_point[3] = { std::numeric_limits<float>::lowest(), 
                                   std::numeric_limits<float>::lowest(), 
                                   std::numeric_limits<float>::lowest() };

            for (int i = 0; i < n; i++)
            {
                
            }
        }

        void FitCovariance(float* points, float& n, OBB* box) 
        {
            // Step 1: Compute centroid
            float mean[3] = {0.0f, 0.0f, 0.0f};

            for(int i = 0; i < n; i++)
            {
                mean[0] += points[i * 8 + 0];
                mean[1] += points[i * 8 + 1];
                mean[2] += points[i * 8 + 2];
            }
            
            Math::SubVecS3(mean, mean, n);

            // Step 2: Compute covariance matrix
            Mat3x3 covariance;
            for(int i = 0; i < n; i++)
            {
                float x = points[i * 8 + 0] - mean[0];
                float y = points[i * 8 + 1] - mean[1];
                float z = points[i * 8 + 2] - mean[2];

                covariance(0, 0) += x * x;
                covariance(0, 1) += x * y;
                covariance(0, 2) += x * z;

                covariance(1, 0) += y * x;
                covariance(1, 1) += y * y;
                covariance(1, 2) += y * z;

                covariance(2, 0) += z * x;
                covariance(2, 1) += z * y;
                covariance(2, 2) += z * z;
            }

            // Step 3: Compute eigenvectors
            float eigenvalues[3];
            float eigenvectors[3][3];
            JacobiEigenvalues(covariance, eigenvalues, eigenvectors);

            // Store rotation matrix (eigenvectors)
            for (int i = 0; i < 3; ++i)
                for (int j = 0; j < 3; ++j)
                    box->rotation[i][j] = eigenvectors[i][j];

            // Step 4: Transform vertices into this new space
            float minBound[3] = {FLT_MAX, FLT_MAX, FLT_MAX};
            float maxBound[3] = {-FLT_MAX, -FLT_MAX, -FLT_MAX};

            for(int i = 0; i < n; i++) 
            {
                float p0 = points[i * 8 + 0];
                float p1 = points[i * 8 + 1];
                float p2 = points[i * 8 + 2];

                float localP[3] = 
                {
                    eigenvectors[0][0] * (p0 - mean[0]) + eigenvectors[1][0] * (p1 - mean[1]) + eigenvectors[2][0] * (p2 - mean[2]),
                    eigenvectors[0][1] * (p0 - mean[0]) + eigenvectors[1][1] * (p1 - mean[1]) + eigenvectors[2][1] * (p2 - mean[2]),
                    eigenvectors[0][2] * (p0 - mean[0]) + eigenvectors[1][2] * (p1 - mean[1]) + eigenvectors[2][2] * (p2 - mean[2])
                };

                for (int i = 0; i < 3; ++i) 
                {
                    minBound[i] = std::min(minBound[i], localP[i]);
                    maxBound[i] = std::max(maxBound[i], localP[i]);
                }
            }

            // Step 5: Compute final OBB properties
            for (int i = 0; i < 3; ++i) 
            {
                box->center[i] = mean[i] + box->rotation[i][0] * ((maxBound[0] + minBound[0]) / 2.0f)
                                         + box->rotation[i][1] * ((maxBound[1] + minBound[1]) / 2.0f)
                                         + box->rotation[i][2] * ((maxBound[2] + minBound[2]) / 2.0f);
                box->halfExtents[i] = (maxBound[i] - minBound[i]) / 2.0f;
            }
        }
    }
    
    
} // namespace Skynet


#endif // SKYNET_COLLISION_OBB_H