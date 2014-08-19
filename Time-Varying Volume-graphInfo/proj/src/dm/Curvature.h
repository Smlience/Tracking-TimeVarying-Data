#include "util/Vector3.h"
#include "util/matrix3.h"

#define pos2idx(pos,dim) pos.x + pos.y * dim.x + pos.z * dim.x * dim.y
/**
 * Calculates the curvature for each voxel.
 */
inline void calcCurvature(float* result, unsigned char* input, Vector3i dim, unsigned int curvatureType = 2) {
	Vector3i pos;
	unsigned int idx;
	float minCurvature = std::numeric_limits<float>::max();
	float maxCurvature = std::numeric_limits<float>::min();
	for (pos.z = 0; pos.z < dim.z; pos.z++) {
		for (pos.y = 0; pos.y < dim.y; pos.y++) {
			for (pos.x = 0; pos.x < dim.x; pos.x++) {
				idx = pos.x + pos.y * dim.x + pos.z * dim.x * dim.y;
				if (pos.x >= 2 && pos.x < dim.x-2 &&
					pos.y >= 2 && pos.y < dim.y-2 &&
					pos.z >= 2 && pos.z < dim.z-2)
				{
					// fetch necessary data
					float c = input[idx] / 255.0f;
					 
					float r0 = input[idx + 1] / 255.0f;
					float r1 = input[idx + 2] / 255.0f;
					float l0 = input[idx - 1] / 255.0f;
					float l1 = input[idx - 2] / 255.0f;

					float u0 = input[idx + dim.x] / 255.0f;
					float u1 = input[idx + dim.x * 2] / 255.0f;
					float d0 = input[idx - dim.x] / 255.0f;
					float d1 = input[idx - dim.x * 2] / 255.0f;

					float f0 = input[idx + dim.x * dim. y] / 255.0f;
					float f1 = input[idx + dim.x * dim. y * 2] / 255.0f;
					float b0 = input[idx - dim.x * dim. y] / 255.0f;
					float b1 = input[idx - dim.x * dim. y * 2] / 255.0f;

					float ur0 = input[idx + dim.x + 1] / 255.0f;
					float dr0 = input[idx - dim.x + 1] / 255.0f;
					float ul0 = input[idx + dim.x - 1] / 255.0f;
					float dl0 = input[idx - dim.x - 1] / 255.0f;

					float fr0 = input[idx + dim.x * dim. y + 1] / 255.0f;
					float br0 = input[idx - dim.x * dim. y + 1] / 255.0f;
					float fl0 = input[idx + dim.x * dim. y - 1] / 255.0f;
					float bl0 = input[idx - dim.x * dim. y - 1] / 255.0f;

					float uf0 = input[idx + dim.x + dim.x * dim. y] / 255.0f;
					float ub0 = input[idx + dim.x - dim.x * dim. y] / 255.0f;
					float df0 = input[idx - dim.x + dim.x * dim. y] / 255.0f;
					float db0 = input[idx - dim.x - dim.x * dim. y] / 255.0f;

					Vector3f gradient = Vector3f(l0-r0,d0-u0,b0-f0);

					float gradientLength = gradient.norm();
					if (gradientLength == 0.0f) gradientLength = 1.0f;

					Vector3f n = -gradient / gradientLength;

					Matrix3f nxn; // matrix to hold the outer product of n and n^T
					for (int i=0; i<3; ++i)
						for (int j=0; j<3; ++j)
							nxn[i][j] = n[i]*n[j];

					Matrix3f P;
					P.identity();
					P -= nxn;

					// generate Hessian matrix
					float fxx = (((r1-c)/2.0f)-((c-l1)/2.0f))/2.0f;
					float fyy = (((u1-c)/2.0f)-((c-d1)/2.0f))/2.0f;
					float fzz = (((f1-c)/2.0f)-((c-b1)/2.0f))/2.0f;
					float fxy = (((ur0-ul0)/2.0f)-((dr0-dl0)/2.0f))/2.0f;
					float fxz = (((fr0-fl0)/2.0f)-((br0-bl0)/2.0f))/2.0f;
					float fyz = (((uf0-ub0)/2.0f)-((df0-db0)/2.0f))/2.0f;
					Matrix3f H(fxx, fxy, fxz, fxy, fyy, fyz, fxz, fyz, fzz);

					Matrix3f G = -(P*H*P / gradientLength);

					// compute trace of G
					float trace = G.m[0][0] + G.m[1][1] + G.m[2][2];

					// compute Frobenius norm of G
					float F = 0.0f;
					for (int i=0; i<3; ++i)
						for (int j=0; j<3; ++j)
							F += powf(std::abs(G.m[i][j]), 2.0f);
					//F = sqrt(F);
// 					float kappa1 = G.m[0][0];
// 					float kappa2 = G.m[1][1];
					F = 2.0f * F - powf(trace, 2.0f);
					F = (F < 0) ? 0 : sqrtf(F);
					float kappa1 = (trace + F) / 2.0f;
					float kappa2 = (trace - F) / 2.0f;

// 					float kappa1 = (trace + sqrtf(2.0f * powf(F,2.0f) - powf(trace,2.0f))) / 2.0f;
// 					float kappa2 = (trace - sqrtf(2.0f * powf(F,2.0f) - powf(trace, 2.0f))) / 2.0f;
					//float tmp = 2.0f * powf(F2,2.0f) - powf(trace, 2.0f);
// 					float tmp = 2.0f * F - powf(trace, 2.0f);
// 					float t1 = sqrtf(tmp);
// 					if (tmp < 0)
// 					{
// 						printf("shit!!! %f %f", kappa1, kappa2);
// 						if (tmp < 0)
// 						{
// 							printf(" >> %f", tmp / tmp);
// 						}
// 						else if (tmp > 0)
// 						{
// 							printf(" >> MAX %f", tmp / tmp);
// 						}
// 						printf("\n");
// 
// 					}

					float curvature = 0.f;
					if (curvatureType == 0) // first principle
						curvature = kappa1;
					else if (curvatureType == 1) // second principle
						curvature = kappa2;
					else if (curvatureType == 2) // mean
						curvature = (kappa1+kappa2)/2.0f;
					else if (curvatureType == 3) // Gaussian
						curvature = kappa1*kappa2;
					result[idx] = curvature;

					if (curvature < minCurvature) minCurvature = curvature;
					else if (curvature > maxCurvature) maxCurvature = curvature;
				} else {
					result[idx] = 0.0f;
				}
			}
		}
	}

	printf("MIN:%f MAX:%f\n", minCurvature, maxCurvature);
	// scale curvature to lie in interval [0.0,1.0], where 0.5 equals zero curvature
	for (pos.z = 0; pos.z < dim.z; pos.z++) {
		for (pos.y = 0; pos.y < dim.y; pos.y++) {
			for (pos.x = 0; pos.x < dim.x; pos.x++) {
				idx = pos.x + pos.y * dim.x + pos.z * dim.x * dim.y;
				float c = result[idx];
				if (c < 0.0f) c /= -minCurvature;
				else if (c >= 0.0f) c /= maxCurvature;
				c /= 2.0f;
				c += 0.5f;
				result[idx] = c;
			}
		}
	}
}