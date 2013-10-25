#include <math.h>
#include <stdio.h>
#include "vect.h"
#include "coef.h"
#include "Particle.h"
#include "Demon.h"
#include "pov.h"
/*
float GetKr(const Particle *p1, const Particle *p2)
{
  float result = 0.0f;
  result = sqrt(1.0/p1->radius + 1.0/p2->radius);
  result *= (1-p1->poission*p1->poission)/p1->young
    + (1-p2->poission*p2->poission)/p2->young;
  return 0.75f/ result;
}
*/
typedef void (*InitDemonFunc)(Demon *);
enum {
  INIT_PUSH_SHPERE,
  INIT_FALL_SHPERE,
  INIT_THROW_SHPERE,
  INIT_FUNC_NUM
};
char filenameList[INIT_FUNC_NUM][128] = {
  "demon-push",
  "demon-fall",
  "demon-throw",
};

const static unsigned PART_NUM = 2;

int main(int argc, char *argv[])
{
	char strFileName[512] = "";
	
	Demon d1;
	//InitDemon2Gran(&d1);
  vect or;
  vectSetValue(or, 1.0, 1.0, 1.0);

	//InitDemonBox(&d1, or, 4, 4, 4);
	//InitDemonGroGra(&d1);
  const InitDemonFunc initList[INIT_FUNC_NUM] = {
    InitDemonPush,
    InitDemonFall,
    InitDemonThrow
  };
      

	pov pov1;
  pov1.num_light = 0;
  pov1.num_include= 0;
	//vectSetValue(&(pov1.camera.location), 10, 10, 10);
	//vectSetValue(&(pov1.camera.lookAt), 0, 0, 0);
	vectSetValue(&(pov1.camera.location), 10, 15, 5);
	vectSetValue(&(pov1.camera.lookAt), 10, 0, 0);

	pov_light light1;
	vectSetValue(&(light1.location), 10, -10, 10);
	lightSetColor(&light1, "White");
	povAddLight(&pov1, &light1);
	vectSetValue(&(light1.location), -10, 10, 10);
	lightSetColor(&light1, "White");
	povAddLight(&pov1, &light1);
	vectSetValue(&(light1.location), -10, -10, 10);
	lightSetColor(&light1, "White");
	povAddLight(&pov1, &light1);

	vectSetValue(&(light1.location), 10, 10, 10);
	lightSetColor(&light1, "White");
	povAddLight(&pov1, &light1);
	povAddInclude(&pov1, "colors");

  for(unsigned iCurDemon = 0; iCurDemon < INIT_FUNC_NUM; iCurDemon ++) {
    (initList[iCurDemon])(&d1);

    pov1.dem_scene = &d1;
    unsigned output_rate = (unsigned)(1.0/60.0/d1.time_step);
    char cmd[256];
    sprintf(cmd, "mkdir pov_out/%s", filenameList[iCurDemon]);
    system(cmd);
    sprintf(cmd, "mkdir pov_out/%s/pov", filenameList[iCurDemon]);
    system(cmd);
    for (unsigned iFrame = 0; iFrame < 4800; iFrame ++) {
      if (iFrame % output_rate == 0) {
        //sprintf(strFileName, "/home/hammurabi/toShare/demon-1/pov/GroundGranular%03u.pov", iFrame/output_rate);
        sprintf(strFileName, "out/%s/pov/%03u.pov", filenameList[iCurDemon], iFrame/output_rate);
        //printf("%f\t%f\t%f\n", d1.sand[0].component[0].position[0], d1.sand[0].component[0].position[1], d1.sand[0].component[0].position[2]);
        //print_vect(d1.sand->component[0].position, "");
        if( 0 == povSave(&pov1, strFileName)) {
          printf("Can't open file: %s\n", strFileName);
          return;
        }
      }
      ComputeForce(&d1);
      TimeIntergration(&d1);
    }

    FreeDemon(&d1);
  }
	povFree(&pov1);
  return 0;
}
