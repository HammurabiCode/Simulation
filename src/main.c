#include <time.h>
#include <math.h>
#include <stdio.h>
#include "vect.h"
#include "Demon.h"
#include "pov.h"

int main(int argc, char *argv[])
{
  clock_t start_time, end_time;

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

  //unsigned iCurDemon = INIT_INTER_CUBE_GRAN;
  //unsigned iCurDemon = INIT_CUBE;
  //unsigned iCurDemon = INIT_GRAN_CUBE;
  unsigned iCurDemon = INIT_GRAN_2_COLLIDE;
  //unsigned iCurDemon = INIT_GRAN_BOX;
  //unsigned iCurDemon = INIT_GRAN_PILE;
  //unsigned iCurDemon = INIT_SPHE_BOX;
  //unsigned iCurDemon = INIT_GRAN_ONE_NOR;
  //unsigned iCurDemon = INIT_GRAN_ONE_SHEAR;
  //unsigned iCurDemon = INIT_FALL_SPHERE;
  //for(iCurDemon = 0; iCurDemon < INIT_FUNC_NUM; iCurDemon ++) 
  {
    start_time = clock();
    (initList[iCurDemon])(&d1);
    end_time = clock();
    printf("time to init:%d-%d=%d\n", end_time, start_time, end_time - start_time);

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

      start_time = clock();

      ComputeForce(&d1);

      end_time = clock();
      /*
      printf("*****************\n");
      printf("time to compute force:%d-%d=%d\n", end_time, start_time, end_time - start_time);
      */
      start_time = clock();

      TimeIntergration(&d1);

      /*
      end_time = clock();
      printf("time to time intergration:%d-%d=%d\n", end_time, start_time, end_time - start_time);
      */
    }
    FreeDemon(&d1);
  }
  free(strFileName);
	povFree(&pov1);
  return 0;
}
