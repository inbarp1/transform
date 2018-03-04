#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "ml6.h"
#include "display.h"
#include "draw.h"
#include "matrix.h"
#include "parser.h"


/*======== void parse_file () ==========
Inputs:   char * filename 
          struct matrix * transform, 
          struct matrix * pm,
          screen s
Returns: 
Goes through the file named filename and performs all of the actions listed in that file.
The file follows the following format:
     Every command is a single character that takes up a line
     Any command that requires arguments must have those arguments in the second line.
     The commands are as follows:
         line: add a line to the edge matrix - 
	     takes 6 arguemnts (x0, y0, z0, x1, y1, z1)
	      ident: set the transform matrix to the identity matrix - 
	       scale: create a scale matrix, 
	           then multiply the transform matrix by the scale matrix - 
		       takes 3 arguments (sx, sy, sz)
		        translate: create a translation matrix, 
			    then multiply the transform matrix by the translation matrix - 
			        takes 3 arguments (tx, ty, tz)
				 rotate: create an rotation matrix,
				     then multiply the transform matrix by the rotation matrix -
				         takes 2 arguments (axis, theta) axis should be x y or z
					  apply: apply the current transformation matrix to the 
					      edge matrix
					       display: draw the lines of the edge matrix to the screen
					           display the screen
						    save: draw the lines of the edge matrix to the screen
						        save the screen to a file -
							    takes 1 argument (file name)
							     quit: end parsing
See the file script for an example of the file format
IMPORTANT MATH NOTE:
the trig functions int math.h use radian mesure, but us normal
humans use degrees, so the file will contain degrees for rotations,
be sure to conver those degrees to radians (M_PI is the constant
for PI)
====================*/
void parse_file ( char * filename, 
                  struct matrix * transform, 
                  struct matrix * edges,
                  screen s) {

  FILE *f;
  char line[256];
  struct matrix * temporary;
  clear_screen(s);
   color c;
   c.green=0;
   c.red=255;
   c.blue=0;

  if ( strcmp(filename, "stdin") == 0 ) 
    f = stdin;
  else
    f = fopen(filename, "r");
  while ( fgets(line, 255, f) != NULL ) {
    if (line[strlen(line)-1]=='\n') line[strlen(line)-1]='\0';
    printf(":%s:\n",line);
    
    if(!strcmp(line, "line")){
      fgets(line,255,f);
      double x0=0;
      double x1=0;
      double y0=0;
      double y1=0;
      double z0=0;
      double z1=0;
      if (line[strlen(line)-1]=='\n'){
	line[strlen(line)-1]='\0';
      }
      sscanf(line,"%lf %lf %lf %lf %lf %lf",&x0,&y0,&z0,&x1,&y1,&z1);
      add_edge(edges,x0,y0,z0,x1,y1,z1);
    }
    else if (!strcmp(line,"quit")) {
      exit(0);
    }
    else if (!strcmp(line,"apply")) {
      matrix_mult(transform,edges);
    }
    else if (!strcmp(line,"save")) {
      fgets(line,255,f);
      if (line[strlen(line)-1]=='\n'){
	line[strlen(line)-1]='\0';
      }
      clear_screen(s);
      draw_lines(edges,s,c);
      save_extension(s,line);
    }
    else if (!strcmp(line,"display")) {
      clear_screen(s);
      draw_lines(edges,s,c);
      display(s);
    } 
    else if(!strcmp(line, "ident")){
      ident(transform);
    }
    else if (!strcmp(line,"scale")) {
      fgets(line,255,f);
      if (line[strlen(line)-1]=='\n'){
	line[strlen(line)-1]='\0';
      }
      double scalex=0;
      double scaley=0;
      double scalez=0;
      sscanf(line,"%lf %lf %lf",&scalex,&scaley,&scalez);
      temporary = make_scale(scalex,scaley,scalez);
      matrix_mult(tempprary,transform);
      free_matrix(temporary);
    }
    else if (!strcmp(line,"move")) {
      fgets(line,255,f);
      if (line[strlen(line)-1]=='\n'){
	line[strlen(line)-1]='\0';
      }
      double tranx=0;
      double trany=0;
      double tranz=0;
      sscanf(line,"%lf %lf %lf",&tranx,&trany,&tranz);
      temporary=make_translate(tranx,trany,tranz);
      matrix_mult(temporary,transform);
      free_matrix(temporary);
    }
    else if (!strcmp(line,"rotate")) {
      char xyz[2];
      double theta;
      fgets(line,255,f);
      if (line[strlen(line)-1]=='\n'){
	line[strlen(line)-1]='\0';
      }
      sscanf(line,"%s %lf",xyz,&theta);
      if (xyz[0]=='x') {
	temporary=make_rotX(theta);
      }
      if (xyz[0]=='y') {
	temporary=make_rotY(theta);
      }
      if (xyz[0]=='z') {
	temporary=make_rotZ(theta);
      }
      matrix_mult(temporary,transform);
      free_matrix(temporary);
    }
  }
}
