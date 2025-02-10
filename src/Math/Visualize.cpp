#include "Visualize.h"

namespace Skynet
{
    namespace Math
    {
        void PrintMat(float* mat, int n, int m) {
            std::ostringstream o;
            o << std::fixed << std::setprecision(3);
            for (int i=0; i<n; i++) {
                for (int j=0; j<m; j++) {
                    o << mat[i * m + j] << ' ';
                } o << '\n';
            }
            LOG("{}", o.str());
        }

        void PrintMat4(float* m) {
            std::ostringstream o;
            o << std::fixed << std::setprecision(3);
            for (int i = 0; i < 4; i++) {
                for (int j = 0; j < 4; j++) {
                    o << m[j * 4 + i] << ' ';
                } o << '\n';
            } o << '\n';
            LOG("{}", o.str());
        }

        void PrintMat3(float* m) {
            std::ostringstream o;
            o << std::fixed << std::setprecision(3);
            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 3; j++) {
                    o << m[j * 3 + i] << ' ';
                } o << '\n';
            }
            LOG("{}", o.str());
        }

        void PrintVec4(float* v) {
            std::ostringstream o;
            o << std::fixed << std::setprecision(3);
            o << v[0] << ' ' << v[1] << ' ' << v[2] << ' ' << v[3];
            INFO("{}", o.str());
        }
            
        void PrintVec3(float* v) {
            std::ostringstream o;
            o << std::fixed << std::setprecision(3);
            o << v[0] << ' ' << v[1] << ' ' << v[2];
            INFO("{}", o.str());
        }

        void PrintVec(float* v, int n) {
            std::ostringstream o;
            o << std::fixed << std::setprecision(3);
            for (int i = 0; i < n; i++) {
                o << v[i] << ' ';
            } INFO("{}", o.str());
        }
    
    } // namespace Math

} // namespace Skynet
