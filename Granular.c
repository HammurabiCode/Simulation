#include "Granular.h"
#include "vect.h"

void InitBoxGranular(Granular *gran, float bigR, float smallR,
    float density, const vect *pos)
{
  //--------------------------------
  //quatenion inertia
  copyVect(&(gran->position), pos);
  setVectZero(&(gran->acceleration));
  setVectZero(&(gran->angularMomentum));

  //--------------------------------
  if(gran->component != NULL)
  {
    free(gran->component);
  }
  gran->num = 9;
  gran->component = (Particle*)malloc(sizeof(Particle)*gran->num);
  //--------------------------------
  InitParticle(gran->component, pos, bigR, GranularDensity);
  float dist = bigR + smallR;
  vect offset;
  for(unsigned i = 0; i < 8; i++)
  {
    int x, y, z;
    x = (i&0x01) == 0 ? 1 : -1;
    y = (i&0x02) == 0 ? 1 : -1;
    z = (i&0x04) == 0 ? 1 : -1;
    setValue(&offset, x*dist, y*dist, z*dist);
    add(&offset, pos);
    InitParticle(gran->component+i+1, &offset, smallR, GranularDensity);
  }
}
