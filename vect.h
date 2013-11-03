#ifndef VECT_H_H
#define VECT_H_H
#include <math.h>
#include "common.h"

typedef float real;
typedef float vect[3];
const static vect GRAVITY = {0, 0, -9.8};
//-------------------------------------------------------------------------
static inline void vectSetZero(vect v)
{
  for(int i = 0; i < 3; i ++)
    v[i] = 0;
}
//-------------------------------------------------------------------------
static inline void vectSetValue(vect v, float x, float y, float z)
{
  v[0] = x; v[1] = y; v[2] = z;
}
//-------------------------------------------------------------------------
static inline void vectCopy(vect dst, const vect src)
{
  for(int i = 0; i < 3; i ++)
    dst[i] = src[i];
}
//-------------------------------------------------------------------------
static inline float vectGetLength(const vect v)
{
  return sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
}
//-------------------------------------------------------------------------
static inline void vectAddCrossProduct(vect c, const vect a, const vect b)
{
  c[0] += a[1]*b[2] - a[2]*b[1];
  c[1] += -(a[0]*b[2] - a[2]*b[0]);
  c[2] += a[0]*b[1] - a[1]*b[0];
}
//-------------------------------------------------------------------------
static inline void vectCrossProduct(vect c, const vect a, const vect b)
{
  c[0] = a[1]*b[2] - a[2]*b[1];
  c[1] = -(a[0]*b[2] - a[2]*b[0]);
  c[2] = a[0]*b[1] - a[1]*b[0];
}
//-------------------------------------------------------------------------
static inline float vectDotProduct(const vect a, const vect b)
{
  return a[0]*b[0] + a[1]*b[1] + a[2]*b[2];
}
//-------------------------------------------------------------------------
static inline void vectScaleAdd(vect v, const vect v0, float s)
{
  for(int i = 0; i < 3; i ++)
    v[i] += v0[i]*s;
}
//-------------------------------------------------------------------------
static inline void vectAddTo(vect v, const vect v1, const vect v2)
{
  for(int i = 0; i < 3; i ++)
    v[i] = v1[i] + v2[i];
}
static inline void vectAdd(vect v, const vect v0)
{
  vectAddTo(v, v, v0);
}
//-------------------------------------------------------------------------
static inline void vectSubstractTo(vect v, const vect v1, const vect v2)
{
  for(int i = 0; i < 3; i ++)
    v[i] = v1[i] - v2[i];
}
//-------------------------------------------------------------------------
static inline void vectSubstract(vect v, const vect v0)
{
  vectSubstractTo(v, v, v0);
}
//-------------------------------------------------------------------------
static inline void vectScaleTo(vect v, const vect v0, float s)
{
  for(int i = 0; i < 3; i ++)
    v[i] = v0[i]*s;
}
//-------------------------------------------------------------------------
static inline void vectScale(vect v, float s)
{
  vectScaleTo(v, v, s);
}
//-------------------------------------------------------------------------
static inline float vectGetDistance(const vect v1, const vect v2)
{ 
  vect t;
  vectSubstractTo(t, v1, v2);
  return vectGetLength(t);
}
//-------------------------------------------------------------------------
static inline float vectNormalize(vect a)
{
  float len = vectGetLength(a);
  if(len < ZERO)
    return 0.0f;
  vectScale(a, 1.0f/len);
  return len;
}
//-------------------------------------------------------------------------
static inline vectCheckZero(vect v)
{
  for(int i = 0; i < 3; i ++) {
    if (fabs(v[i]) < ZERO)
      v[i] = 0;
  }
}
//-------------------------------------------------------------------------
static inline void print_vect(const vect v, const char *str)
{
  printf("%s:\t%f\t%f\t%f", str, v[0], v[1], v[2]);
}
//-------------------------------------------------------------------------
#endif
