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
static inline void getDeltaQuat(quat q, const vect w, float ts)
{
  copyVect(GetVofQ(q), w);
  float thelta = normalizeVect(GetVofQ(q)) * ts * 0.5;
  q[0] = cos(thelta);
  scaleVect((vect)(q+1), sin(thelta));
  return;
}
//-------------------------------------------------------------------------
static inline void updateQuat(quat q, const quat delta)
{
  quat q0;
  copyQuat(q0, q);
  q[0] = q0[0]*delta[0] - dotProductVect(GetVofQ(delta), GetVofQ(q0));
  crossProductVect(GetVofQ(q), GetVofQ(delta), GetVofQ(q0));
  scaleAddVect(GetVofQ(q), GetVofQ(q0), delta[0]);
  scaleAddVect(GetVofQ(q), GetVofQ(delta), q0[0]);
}

#endif
