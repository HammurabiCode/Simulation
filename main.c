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
	char strFileName[192] = "";
	
  /*
  mat3x3 matOne, matOneInv, matMul;
  matOne[0][0] = 1; matOne[0][1] = 2; matOne[0][2] = 3;
  matOne[1][0] = 2; matOne[1][1] = 2; matOne[1][2] = 1;
  matOne[2][0] = 3; matOne[2][1] = 4; matOne[2][2] = 3;
  matInv(matOneInv, matOne);
  matMulMat(matMul, matOne, matOneInv);
  matPrint(matMul);
  */
	Demon d1;
	//InitDemon2Gran(&d1);
  vect or;
  vectSetValue(or, 1.0, 1.0, 1.0);

	//InitDemonBox(&d1, or, 4, 4, 4);
	InitDemonGroGra(&d1);

	pov pov1;
  pov1.num_light = 0;
  pov1.num_include= 0;
	vectSetValue(&(pov1.camera.location), 10, 10, 10);
	vectSetValue(&(pov1.camera.lookAt), 0, 0, 0);
	pov_light light1;
	vectSetValue(&(light1.location), 10, 10, 10);
	lightSetColor(&light1, "White");
	povAddLight(&pov1, &light1);
	povAddInclude(&pov1, "colors");
	pov1.dem_scene = &d1;
	for (unsigned iFrame = 0; iFrame < 300; iFrame ++) {
		if (iFrame % 10 == 0) {
			sprintf(strFileName, "pov_out/demon-1/pov/GroundGranular%02u.pov", iFrame/10);
      //print_vect(d1.sand->component[0].position, "");
      povSave(&pov1, strFileName);
		}
		ComputeForce(&d1);
		TimeIntergration(&d1);
	}

  FreeDemon(&d1);
	povFree(&pov1);
  return 0;
}
