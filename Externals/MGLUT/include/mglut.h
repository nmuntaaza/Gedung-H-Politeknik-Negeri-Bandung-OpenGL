//
// Created by Naufal on 07/01/2019.
//

#ifndef GLUTCL_MGLUT_H
#define GLUTCL_MGLUT_H

#include "GL/glut.h"

#define NONE 0
#define FRONT 1
#define BACK 2
#define LEFT 3
#define RIGHT 4
#define ROT_UP 5
#define ROT_DOWN 6
#define ROT_RIGHT 7
#define ROT_LEFT 8

void mGlClearColor(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);
void mGlColor3f(GLfloat red, GLfloat green, GLfloat blue);
void flatCube(float x1,float y1,float z1,float x2,float y2,float z2);
void skyBox();


#endif //GLUTCL_MGLUT_H
