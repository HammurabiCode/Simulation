#ifndef QUAT_H_H
#define QUAT_H_H
#include <math.h>
#include "vect.h"

typedef real quat[4];
#define GetVofQ(q) ((vect)(q+1))

//-------------------------------------------------------------------------
static inline void setQuatZero(quat v)
{
  for(int i = 0; i < 4; i ++)
    v[i] = 0;
}
//-------------------------------------------------------------------------
static inline void copyQuat(quat dst, const quat src)
{
  for(int i = 0; i < 4; i ++)
    dst[i] = src[i];
}
//-------------------------------------------------------------------------
static inline void getVectOfQuat(vect v, const quat q)
{
	for(int i = 0; i < 3; i ++) {
		v[i] = q[i+1];
	}
}
//-------------------------------------------------------------------------
static inline void setVectOfQuat(quat q, const vect v)
{
	for(int i = 0; i < 3; i ++) {
		q[1+i] = v[i];
	}
}
//-------------------------------------------------------------------------
static inline void getDeltaQuat(quat q, const vect w, float ts)
{
	vect vt;
	copyVect(vt, w);
  float thelta = normalizeVect(vt) * ts * 0.5;
  q[0] = cos(thelta);
  scaleVect(vt, sin(thelta));
	setVectOfQuat(q, vt);
}
//-------------------------------------------------------------------------
static inline void updateQuat(quat q, const quat delta)
{
	vect v0, v1, v2;
  getVectOfQuat(v0, delta);
  getVectOfQuat(v1, q);
  q[0] = q[0]*delta[0] - dotProductVect(v1, v0);
  crossProductVect(v2, v0, v1);
  scaleAddVect(v2, v1, delta[0]);
  scaleAddVect(v2, v0, q[0]);
  setVectOfQuat(q, v2);
}

#endif
