#ifndef POV_H_H
#define POV_H_H
#include <stdio.h>
#include "vect.h"
#include "Demon.h"
//-------------------------------------------------------------------------
typedef struct {
  vect location;
  vect lookAt;
} pov_camera;
//-------------------------------------------------------------------------
typedef struct {
  vect location;
  char color[8];
} pov_light;
//-------------------------------------------------------------------------
typedef struct {
  vect location;
  char color[8];
} pov_plane;
typedef char* pov_include;
//-------------------------------------------------------------------------
typedef struct {
  pov_camera camera;
  pov_light *lights;
  unsigned num_light;
  pov_include *include;
  unsigned num_include;
  Demon *dem_scene;
} pov;
//-------------------------------------------------------------------------
static inline void 
cameraSave(const pov_camera *cam, FILE * fp)
{
  fprintf(fp, "camera {\n\tright x*image_width/image_height\n\tlocation <%f, %f, %f>\n\tlook_at <%f, %f, %f>\n}\n",
      cam->location[0], cam->location[1], cam->location[2],
      cam->lookAt[0], cam->lookAt[1], cam->lookAt[2]);
}
//-------------------------------------------------------------------------
static inline void 
lightSave(const pov_light *l, FILE * fp)
{
  fprintf(fp, "light_source {\n\t<%f, %f, %f>\n\tcolor %s\n}\n",
      l->location[0], l->location[1], l->location[2],
      l->color);
}
static inline void 
lightSetColor(pov_light *l, const char* c)
{
	memcpy(l->color, c, sizeof(char)*strlen(c)+1);
}
//-------------------------------------------------------------------------
static inline void 
includeSave(const pov_include inc, FILE * fp)
{
  fprintf(fp, "#include \"%s.inc\"\n", inc);
}
static inline void 
povAddCamera(pov *p, const pov_camera* cam)
{
  memcpy(&(p->camera), cam, sizeof(pov_camera));
}
static inline void 
povAddInclude(pov *p, const char* inc)
{
  if(p->num_include != 0) {
    int old_size = sizeof(pov_include)*(p->num_include);
    pov_include *new_includes = (pov_include*)malloc(old_size+sizeof(pov_include));
    memcpy(new_includes+1, p->include, old_size);
    free(p->include);
    p->include = new_includes;
  } else {
    p->include = (pov_include*)malloc(sizeof(pov_include));
  }
  p->num_include++;
  int new_size = sizeof(char)*(strlen(inc)+1);
  p->include[0] = (pov_include)malloc(new_size);
  memcpy(p->include[0], inc, new_size);
}
static inline void 
povAddLight(pov *p, const pov_light* lig)
{
  if(p->num_light != 0) {
    int old_size = sizeof(pov_light)*(p->num_light);
    pov_light *new_lights= (pov_light*)malloc(old_size+sizeof(pov_light));
    memcpy(new_lights+1, p->lights, old_size);
    free(p->lights);
    p->lights = new_lights;
  } else {
    p->lights = (pov_light*)malloc(sizeof(pov_light));
  }
  p->num_light++;
  memcpy(p->lights, lig, sizeof(pov_light));
}
//-------------------------------------------------------------------------
static
void povSave(const pov *p, const char *filename)
{
  FILE *fp = fopen(filename, "wt");
  for(unsigned i = 0; i < p->num_include; i++) {
    includeSave(p->include[i], fp);
  }
  for(unsigned i = 0; i < p->num_light; i++) {
    lightSave(p->lights+i, fp);
  }
  cameraSave(&(p->camera), fp);
	if(p->dem_scene != NULL) {
		SaveDemon(p->dem_scene, fp);
	}
	fprintf(fp, "plane {\n\t<0, 1, 0>, -1\n\tpigment {checker color Black, color White}\n}\n");
/*
*/
  fclose(fp);
}
//-------------------------------------------------------------------------
#endif
