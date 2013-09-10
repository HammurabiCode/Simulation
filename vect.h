#ifndef VECT_H_H
#define VECT_H_H
#include <math.h>
typedef struct
{
  float x;
  float y;
  float z;
} vect;
#define ZERO (1.0e-10)
//-------------------------------------------------------------------------
static inline void setValue(vect *v, float x, float y, float z)
{
  v->x = x; v->y = y; v->z = z;
}
//-------------------------------------------------------------------------
static inline void copyVect(vect *dst, const vect *src)
{
  dst->x = src->x;
  dst->y = src->y;
  dst->z = src->z;
}
//-------------------------------------------------------------------------
static inline float getLength(const vect *v)
{
  return sqrt(v->x*v->x + v->y*v->y + v->z*v->z);
}
//-------------------------------------------------------------------------
static inline float dotProduct(const vect *a, const vect *b)
{
  return a->x*b->x + a->y*b->y + a->z*b->z;
}
//-------------------------------------------------------------------------
static inline void scaleAdd(vect *v, const vect *v0, float s)
{
  v->x += v0->x*s;
  v->y += v0->y*s;
  v->z += v0->z*s;
}
//-------------------------------------------------------------------------
static inline void addTo(vect *v, const vect *v1, const vect *v2)
{
  v->x = v1->x + v2->x;
  v->y = v1->y + v2->y;
  v->z = v1->z + v2->z;
}
static inline void add(vect *v, const vect *v0)
{
  addTo(v, v, v0);
}
//-------------------------------------------------------------------------
static inline void substractTo(vect *a, const vect *a1, const vect *a2)
{
  a->x = a1->x - a2->x;
  a->y = a1->y - a2->y;
  a->z = a1->z - a2->z;
}
//-------------------------------------------------------------------------
static inline void substract(vect *v, const vect *v0)
{
  substractTo(v, v, v0);
}
//-------------------------------------------------------------------------
static inline void scaleTo(vect *v, const vect *v0, float s)
{
  v->x = v0->x*s;
  v->y = v0->y*s;
  v->z = v0->z*s;
}
//-------------------------------------------------------------------------
static inline void scale(vect *v, float s)
{
  scaleTo(v, v, s);
}
//-------------------------------------------------------------------------
static inline float getDistance(const vect *v1, const vect *v2)
{ 
  vect t;
  substractTo(&t, v1, v2);
  return getLength(&t);
}
//-------------------------------------------------------------------------
static inline float normalize(vect *a)
{
  float len = getLength(a);
  if(len < ZERO)
    return 0.0f;
  scale(a, 1.0f/len);
  return len;
}
//-------------------------------------------------------------------------
static inline void print_vect(const vect *v, const char *str)
{
  printf("%s: x=%f, y=%f, z=%f.", str, v->x, v->y, v->z);
}
//-------------------------------------------------------------------------
#endif
