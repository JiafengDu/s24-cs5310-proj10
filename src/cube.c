/*
  Bruce A. Maxwell
  Fall 2014

  Example code for drawing a single cube

  C Version
*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "graphics.h"

int main(int argc, char *argv[]) {
  Image *src;
  Matrix VTM;
  Matrix GTM;
  Module *cube, *sphere, *formation, *scene;
  int rows = 360;
  int cols = 640;

  Color White, Grey, DkGrey, blue, green, purple, red;

  DrawState *ds;
  View3D view;

  Lighting *light;

	color_set( &White, 1.0, 1.0, 1.0 );
	color_set( &Grey, 0.6, 0.62, 0.64 );
	color_set( &DkGrey, 0.1, 0.1, 0.1 );
  color_set( &blue, .1, .2, .8);
  color_set( &green, .2, 0.7, 0.3 );
  color_set( &purple, 0.6, 0.1, 0.7 );
  color_set( &red, 0.75, 0.3, 0.3 );

  // initialize the image
  src = image_create(rows, cols);

  // initialize matrices
  matrix_identity(&GTM);
  matrix_identity(&VTM);

  // set the View parameters
  point_set3D(&(view.vrp), 7, 7, -10.0);
  vector_set(&(view.vpn), -5, -6, 7);
  vector_set(&(view.vup), 0.0, 1.0, 0.0);
  view.d = 2.0;
  view.du = 1.6;
  view.dv = .9;
  view.f = 0.0;
  view.b = 15;
  view.screenx = cols;
  view.screeny = rows;
  matrix_setView3D(&VTM, &view);

  // print out VTM
  printf("Final VTM: \n");
  matrix_print(&VTM, stdout);

  // make a simple cube module
  cube = module_create();
  sphere = module_create();
  formation = module_create();
  scene = module_create();

  module_scale( cube, 1.0, 0.5, 0.5 );
  module_scale( sphere, 0.5, 0.5, 0.5 );

  // this would color the cube in ShadeConstant mode
  module_color( cube, &White );

	// set body and surface color values for ShadeGouraud mode
	module_bodyColor( cube, &White );
	module_surfaceColor( cube, &DkGrey );

  module_bodyColor( sphere, &blue );
  module_surfaceColor( sphere, &red );

  module_translate(cube, 1.0, 0.0, 0.0);
  module_translate(sphere, -1.0, 0.0, 0.0);
  module_translate(formation, 3, 3, 3);

  // manually add a light source to the Lighting structure
  // put it in the same place as the eye in world space
  light = lighting_create();
  lighting_add( light, LightPoint, &White, NULL, &(view.vrp), 0, 0 );

  // set the shading to Gouraud
  ds = drawstate_create();
	
  point_copy(&(ds->viewer), &(view.vrp));
	ds->shade = ShadeGouraud;
	//	ds->shade = ShadeFlat;
  
  matrix_identity(&GTM);

  // draw the cube
  int max_frame = 20;
	for(int frame=0;frame<max_frame;frame++) {
		char buffer[256];
		float c = cos(M_PI/max_frame);
    float s = sin(M_PI/max_frame);
		matrix_rotateY(&GTM, c, s );
    module_rotateX(sphere, c, s );
    module_rotateZ(cube, c, s );
    module_sphere( sphere, 5);
    module_cube( cube, 1);
    module_module( formation, sphere );
    module_module( formation, cube );
    module_module( scene, formation );
		module_draw(scene, &VTM, &GTM, ds, light, src);

		sprintf(buffer, "../images/complex-frame%03d.ppm", frame);
		image_write(src, buffer);
		image_reset(src);
	}
  // free stuff here
  module_delete( cube );
  image_free( src );
  

  return(0);
}
