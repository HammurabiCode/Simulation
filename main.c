#include <math.h>
#include <stdio.h>
#include "vect.h"
#include "coef.h"
#include "Particle.h"
#include "Demon.h"
#include "pov.h"

int main(int argc, char *argv[])
{
  vect zero;
  vectSetZero(zero);
  float a = vectGetLength(zero);
  printf("%f\n", abs((int)(a)));
  if (abs(a) > 0.0f) printf("aaa\n");
	char *strFileName = (char*) malloc(sizeof(char)*1024);
	
	Demon d1;
	//InitDemon2Gran(&d1);
  vect or;
  vectSetValue(or, 1.0, 1.0, 1.0);

	//InitDemonBox(&d1, or, 4, 4, 4);
	//InitDemonGroGra(&d1);

	pov pov1;
  pov1.num_light = 0;
  pov1.num_include= 0;
	//vectSetValue(&(pov1.camera.location), 10, 10, 10);
	//vectSetValue(&(pov1.camera.lookAt), 0, 0, 0);
	vectSetValue(&(pov1.camera.location), 0, 15, 5);
	vectSetValue(&(pov1.camera.lookAt), 0, 0, 0);

	pov_light light1;
	vectSetValue(&(light1.location), 20, 0, 20);
	lightSetColor(&light1, "White");
	povAddLight(&pov1, &light1);
	vectSetValue(&(light1.location), -20, 0, 20);
	lightSetColor(&light1, "White");
	povAddLight(&pov1, &light1);
	vectSetValue(&(light1.location), 0, -20, 20);
	lightSetColor(&light1, "White");
	povAddLight(&pov1, &light1);
	vectSetValue(&(light1.location), 0, 20, 20);
	lightSetColor(&light1, "White");
	povAddLight(&pov1, &light1);
	povAddInclude(&pov1, "colors");

  unsigned iCurDemon = INIT_CUBE_BOX;
  //unsigned iCurDemon = INIT_CUBE;
  //unsigned iCurDemon = INIT_GRAN_2_COLLIDE;
  //unsigned iCurDemon = INIT_GRAN_BOX;
  //unsigned iCurDemon = INIT_GRAN_PILE;
  //unsigned iCurDemon = INIT_SPHE_BOX;
  //unsigned iCurDemon = INIT_GRAN_ONE_NOR;
  //unsigned iCurDemon = INIT_GRAN_ONE_SHEAR;
  //unsigned iCurDemon = INIT_FALL_SPHERE;
  //for(iCurDemon = 0; iCurDemon < INIT_FUNC_NUM; iCurDemon ++) 
  {
    (initList[iCurDemon])(&d1);

    pov1.dem_scene = &d1;
    unsigned output_rate = (unsigned)(1.0/25.0/d1.time_step);
    /*
    output_rate = 1;
    */
    char cmd[256];
    sprintf(cmd, "mkdir out/%s", filenameList[iCurDemon]);
    system(cmd);
    sprintf(cmd, "mkdir out/%s/pov", filenameList[iCurDemon]);
    system(cmd);
    sprintf(cmd, "rm out/%s/pov/*.pov", filenameList[iCurDemon]);
    system(cmd);
    unsigned nFrame = 5*25;
    unsigned iFrame = 0;
    for (unsigned iStep = 0; iFrame < nFrame; iStep++) {
      if (iStep % output_rate == 0) {
        sprintf(strFileName, "out/%s/pov/%03u.pov", filenameList[iCurDemon], iFrame);
        if( 0 == povSave(&pov1, strFileName)) {
          printf("Can't open file: %s\n", strFileName);
          return -1;
        }
      /*
      */
        iFrame++;
      }
      ComputeForce(&d1);
      /*
      if (iStep >= 30000 && iStep <= 33280) {
        printf("**********************************%04u\n", iStep);
        unsigned ig = 7;

        //for (unsigned ig = 0; ig < d1.num; ig ++) {
          GranularPrint(d1.sand+ig);
        //}
      }
      */
      TimeIntergration(&d1);
      if (d1.sand[7].position[0] != d1.sand[7].position[0]) {
        printf("%u\n", iStep);
      }
    }
    FreeDemon(&d1);
  }
  free(strFileName);
	povFree(&pov1);
  return 0;
}
