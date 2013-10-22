#ifndef VECT_H_H
#define VECT_H_H
#include <math.h>
#ifndef NULL
#define NULL (0)
#endif
#define ZERO (1.0e-10)
#define PI (3.1415926535358979)

typedef float real;
typedef float vect[3];
//-------------------------------------------------------------------------
static inline void setVectZero(vect v)
{
  for(int i = 0; i < 3; i ++)
    v[i] = 0;
}
//-------------------------------------------------------------------------
static inline void setVectValue(vect v, float x, float y, float z)
{
  v[0] = x; v[1] = y; v[2] = z;
}
//-------------------------------------------------------------------------
static inline void copyVect(vect dst, const vect src)
{
  for(int i = 0; i < 3; i ++)
    dst[i] = src[i];
}
//-------------------------------------------------------------------------
static inline float getVectLength(const vect v)
{
  return sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
}
//-------------------------------------------------------------------------
static inline void addCrossProductVect(vect c, const vect a, const vect b)
{
  c[0] += a[1]*b[2] - a[2]*b[1];
  c[1] += -(a[0]*b[2] - a[2]*b[0]);
  c[2] += a[0]*b[1] - a[1]*b[0];
}
//-------------------------------------------------------------------------
static inline void crossProductVect(vect c, const vect a, const vect b)
{
  c[0] = a[1]*b[2] - a[2]*b[1];
  c[1] = -(a[0]*b[2] - a[2]*b[0]);
  c[2] = a[0]*b[1] - a[1]*b[0];
}
//-------------------------------------------------------------------------
static inline float dotProductVect(const vect a, const vect b)
{
  return a[0]*b[0] + a[1]*b[1] + a[2]*b[2];
}
//-------------------------------------------------------------------------
static inline void scaleAddVect(vect v, const vect v0, float s)
{
  for(int i = 0; i < 3; i ++)
    v[i] += v0[i]*s;
}
//-------------------------------------------------------------------------
static inline void addVectTo(vect v, const vect v1, const vect v2)
{
  for(int i = 0; i < 3; i ++)
    v[i] = v1[i] + v2[i];
}
static inline void addVect(vect v, const vect v0)
{
  addVectTo(v, v, v0);
}
//-------------------------------------------------------------------------
static inline void substractVectTo(vect v, const vect v1, const vect v2)
{
  for(int i = 0; i < 3; i ++)
    v[i] = v1[i] - v2[i];
}
//-------------------------------------------------------------------------
static inline void substractVect(vect v, const vect v0)
{
  substractVectTo(v, v, v0);
}
//-------------------------------------------------------------------------
static inline void scaleVectTo(vect v, const vect v0, float s)
{
  for(int i = 0; i < 3; i ++)
    v[i] = v0[i]*s;
}
//-------------------------------------------------------------------------
static inline void scaleVect(vect v, float s)
{
  scaleVectTo(v, v, s);
}
//-------------------------------------------------------------------------
static inline float getVectDistance(const vect v1, const vect v2)
{ 
  vect t;
  substractVectTo(t, v1, v2);
  return getVectLength(t);
}
//-------------------------------------------------------------------------
static inline float normalizeVect(vect a)
{
  float len = getVectLength(a);
  if(len < ZERO)
    return 0.0f;
  scaleVect(a, 1.0f/len);
  return len;
}
//-------------------------------------------------------------------------
static inline void print_vect(const vect v, const char *str)
{
  printf("%s: x=%f, y=%f, z=%f.", str, v[0], v[1], v[2]);
}
const static vect GRAVITY = {0, 0, -9.8};
//-------------------------------------------------------------------------
#endif
