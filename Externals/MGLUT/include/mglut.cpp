//
// Created by Naufal on 07/01/2019.
//

#include "mglut.h"

void mGlClearColor(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha)
{
    glClearColor(red/255.0f, green/255.0f, blue/255.0f, alpha);
}

void mGlColor3f(GLfloat red, GLfloat green, GLfloat blue)
{
    glColor3f(red/255.0f, green/255.0f, blue/255.0f);
}

void flatCube(float x1, float y1, float z1, float x2, float y2, float z2)
{
	// Depan
	glTexCoord2f(0.0, 0.0);
	glVertex3f(x1, y1, z1);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(x2, y1, z1);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(x2, y2, z1);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(x1, y2, z1);
	
	// Atas
	glTexCoord2f(0.0, 0.0);
	glVertex3f(x1, y2, z1);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(x2, y2, z1);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(x2, y2, z2);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(x1, y2, z2);
	
	// Belakang
	glTexCoord2f(0.0, 0.0);
	glVertex3f(x1, y2, z2);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(x2, y2, z2);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(x2, y1, z2);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(x1, y1, z2);
	
	// Bawah
	glTexCoord2f(0.0, 0.0);
	glVertex3f(x1, y1, z2);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(x2, y1, z2);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(x2, y1, z1);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(x1, y1, z1);
	
	// Samping Kiri
	glTexCoord2f(0.0, 0.0);
	glVertex3f(x1, y1, z1);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(x1, y2, z1);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(x1, y2, z2);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(x1, y1, z2);
	
	// Samping Kanan
	glTexCoord2f(0.0, 0.0);
	glVertex3f(x2, y1, z1);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(x2, y2, z1);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(x2, y2, z2);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(x2, y1, z2);
}
