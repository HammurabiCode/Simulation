#ifndef MAT_H_H
#define MAT_H_H
#include "vect.h"
#include "quat.h"

typedef	real mat3x3[3][3];

//-------------------------------------------------------------------------
static inline void
matSetZero(mat3x3 m) {
	for(int i = 0; i < 3; i ++) {
		for(int j = 0; j < 3; j ++) {
		  m[i][j] = 0;
    }
	}
}

//-------------------------------------------------------------------------
static inline void
matCopy(mat3x3 dst, mat3x3 src)
{
	for(int i = 0; i < 3; i ++) {
		for(int j = 0; j < 3; j ++) {
		  dst[i][j] = src[i][j];
    }
	}
}
//-------------------------------------------------------------------------
static inline void
matSetIdentity(mat3x3 m)
{
	for(int i = 0; i < 3; i ++) {
		for(int j = 0; j < 3; j ++) {
		  m[i][j] = (i == j) ? 1 : 0;
    }
	}
}

//-------------------------------------------------------------------------
static inline void
matMulMat(mat3x3 dst, const mat3x3 a, const mat3x3 b)
{
	for(int i = 0; i < 3; i ++) {
		for(int j = 0; j < 3; j ++) {
			dst[i][j] = 0;
			for(int m = 0; m < 3; m ++) {
				dst[i][j] += a[i][m]*b[m][j];
			}
		}
	}
}

//-------------------------------------------------------------------------
static inline void
matMulVect(vect dst, const mat3x3 a, const vect b)
{
	for(int i = 0; i < 3; i ++) {
    dst[i] = 0;
		for(int j = 0; j < 3; j ++) {
      dst[i] += a[i][j] * b[j];
    }
  }
}

//-------------------------------------------------------------------------
static inline void
matScale(mat3x3 m, real s)
{
	for(int i = 0; i < 3; i ++) {
		for(int j = 0; j < 3; j ++) {
		  m[i][j] *= s;
    }
	}
}
//-------------------------------------------------------------------------
static inline void
matTranspose(mat3x3 dst, const mat3x3 src)
{
	for(int i = 0; i < 3; i ++) {
		for(int j = 0; j < 3; j ++) {
			dst[i][j] = src[j][i];
		}
	}
}
//-------------------------------------------------------------------------
static inline void
matFromQuat(mat3x3 dst, const quat q)
{
  real xx = q[1]*q[1];
  real yy = q[2]*q[2];
  real zz = q[3]*q[3];
  
  real xy = q[1]*q[2];
  real xz = q[1]*q[3];
  real yz = q[2]*q[3];

  real sx = q[0]*q[1];
  real sy = q[0]*q[2];
  real sz = q[0]*q[3];
  dst[0][0] = 1 - 2*(yy + zz);
  dst[0][1] = 2*(xy - sz);
  dst[0][2] = 2*(xz + sy);

  dst[1][0] = 2*(xy + sz);
  dst[1][1] = 1 - 2*(xx + zz);
  dst[1][2] = 2*(yz - sx);

  dst[2][0] = 2*(xz - sy);
  dst[2][1] = 2*(yz + sx);
  dst[2][2] = 1 - 2*(xx + yy);
}
//-------------------------------------------------------------------------
static inline int
matInv(mat3x3 dst, const mat3x3 src)
{
	dst[0][0] = (src[1][1]*src[2][2]-src[1][2]*src[2][1]);
	dst[1][0] = -(src[1][0]*src[2][2]-src[1][2]*src[2][0]);
	dst[2][0] = (src[1][0]*src[2][1]-src[1][1]*src[2][0]);
  //printf("[%f,\t%f,\t%f]\n", dst[0][0], dst[0][1], dst[0][2]);
  
	real hls = 0;
	hls += src[0][0]*dst[0][0];
	hls += src[0][1]*dst[1][0];
	hls += src[0][2]*dst[2][0];
  if(abs(hls) < ZERO) {
    //matCopy(dst, src);
    matSetZero(dst);
    return 0;
  }
  //printf("hls = %f\n", hls);

	dst[0][1] = -(src[0][1]*src[2][2]-src[0][2]*src[2][1])/hls;
	dst[1][1] = (src[0][0]*src[2][2]-src[0][2]*src[2][0])/hls;
	dst[2][1] = -(src[0][0]*src[2][1]-src[2][0]*src[0][1])/hls;

	dst[0][2] = (src[0][1]*src[1][2]-src[1][1]*src[0][2])/hls;
	dst[1][2] = -(src[0][0]*src[1][2]-src[0][2]*src[1][0])/hls;
	dst[2][2] = (src[0][0]*src[1][1]-src[1][0]*src[0][1])/hls;

	dst[0][0] /= hls;
	dst[1][0] /= hls;
	dst[2][0] /= hls;
  return 1;
}
//-------------------------------------------------------------------------
static inline void
matPrint(const mat3x3 src)
{
  printf("[%f,\t%f,\t%f]\n[%f,\t%f,\t%f]\n[%f,\t%f,\t%f]\n",
    src[0][0], src[0][1], src[0][2],
    src[1][0], src[1][1], src[1][2],
    src[2][0], src[2][1], src[2][2]);
}
#endif
