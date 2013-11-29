#ifndef COMMON_H_H
#define COMMON_H_H
typedef unsigned ParticleIndex;
typedef unsigned GranularIndex;
#define ZERO (1.0e-6)
#define PI (3.14159265f)
#define MAX_VELOCITY (100.0)
const static float GRANULAR_DIMENSION = 1.0e-2;
const static float GRAN_TETR_R = 1.0e-2;
const static float GRAN_OCTA_R = 1.0e-2;
const static float GRAN_CUBE_R = 1.0e-2;
const static float GRAN_BOXB_R = 1.0e-2;
const static float GRAN_BOXS_R = 1.0e-2;
#define TO_INT(a) ((int)(a))
#ifndef NULL
#define NULL (0)
#endif

//#define ZERO_GRAVITY
#endif
