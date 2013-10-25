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
static inline void quatGetVect(vect v, const quat q)
{
	for(int i = 0; i < 3; i ++) {
		v[i] = q[i+1];
	}
}
//-------------------------------------------------------------------------
static inline void quatSetVect(quat q, const vect v)
{
	for(int i = 0; i < 3; i ++) {
		q[1+i] = v[i];
	}
}
//-------------------------------------------------------------------------
static inline void getDeltaQuat(quat q, const vect w, float ts)
{
	vect vt;
	vectCopy(vt, w);
  float thelta = vectNormalize(vt) * ts * 0.5;
  q[0] = cos(thelta);
  vectScale(vt, sin(thelta));
	quatSetVect(q, vt);
}
//-------------------------------------------------------------------------
static inline void updateQuat(quat q, const quat delta)
{
	vect v0, v1, v2;
  quatGetVect(v0, delta);
  quatGetVect(v1, q);
  q[0] = q[0]*delta[0] - vectDotProduct(v1, v0);
  vectCrossProduct(v2, v0, v1);
  vectScaleAdd(v2, v1, delta[0]);
  vectScaleAdd(v2, v0, q[0]);
  quatSetVect(q, v2);
}

#endif
