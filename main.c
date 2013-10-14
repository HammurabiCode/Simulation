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


const static unsigned PART_NUM = 2;

int main(int argc, char *argv[])
{
	Demon d1;
	InitDemon(&d1);

	pov pov1;
	setVectValue(&(pov1.camera.location), 10, 10, 10);
	setVectValue(&(pov1.camera.lookAt), 0, 0, 0);
	pov_light light1;
	setVectValue(&(light1.location), 10, 10, 10);
	lightSetColor(&light1, "White");
	povAddLight(&pov1, &light1);
	povAddInclude(&pov1, "colors");
	pov1.dem_scene = &d1;
	povSave(&pov1, "1.pov");

  return 0;
}
