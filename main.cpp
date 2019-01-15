#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <iostream>
#include "mglut.h"

float z_pos = -10.0f;
float xpos = 0;
float zpos = 0;
float rot = 0.0f;
float xx, yy;

int direction = NONE;

GLuint texture[35];

struct Image 
{
	unsigned long sizeX;
	unsigned long sizeY;
	char *data;
};

int ImageLoad(char *filename, Image* image) 
{
	FILE* file;
	
	unsigned long size; // Ukuran image dalam bytes
	unsigned long i; // Standard counter.
	unsigned short int plane; // Number of planes in image

	unsigned short int bpp; // Jumlah bits per pixel
	char temp; // Temporary color storage for var warna sementara untuk memastikan filenya ada

	if ((file = fopen(filename, "rb")) == NULL) 
	{
		printf("File Not Found : %s\n", filename);
		return 0;
	}
	
	// Mencari file header bmp
	fseek(file, 18, SEEK_CUR);
	
	// Read the width
	if ((i = fread(&image->sizeX, 4, 1, file)) != 1) 
	{
		printf("Error reading width from %s.\n", filename);
		return 0;
	}
	
	// Membaca nilai height
	if ((i = fread(&image->sizeY, 4, 1, file)) != 1) 
	{
		printf("Error reading height from %s.\n", filename);
		return 0;
	}
	
	// Menghitung ukuran image(asumsi 24 bits or 3 bytes per pixel).
	size = image->sizeX * image->sizeY * 3;
	
	// Read the planes
	if ((fread(&plane, 2, 1, file)) != 1) 
	{
		printf("Error reading planes from %s.\n", filename);
		return 0;
	}
	if (plane != 1) 
	{
		printf("Planes from %s is not 1: %u\n", filename, plane);
		return 0;
	}
	// Read the bitsperpixel
	if ((i = fread(&bpp, 2, 1, file)) != 1) 
	{
		printf("Error reading bpp from %s.\n", filename);

		return 0;
	}
	if (bpp != 24) 
	{
		printf("Bpp from %s is not 24: %u\n", filename, bpp);
		return 0;
	}
	// Seek past the rest of the bitmap header.
	fseek(file, 24, SEEK_CUR);
	
	// Read the data.
	image->data = (char *) malloc(size);
	if (image->data == NULL) 
	{
		printf("Error allocating memory for color-corrected image data");
		return 0;
	}
	if ((i = fread(image->data, size, 1, file)) != 1) 
	{
		printf("Error reading image data from %s.\n", filename);
		return 0;
	}
	for (i = 0; i < size; i += 3) 
	{ // Membalikan semuan nilai warna (gbr - > rgb)
		temp = image->data[i];
		image->data[i] = image->data[i + 2];
		image->data[i + 2] = temp;
	}
	
	return 1;
}

Image* LoadTexture(char* image_location)
{
	Image* image;
	
	image = (Image*)malloc(sizeof(Image));
	
	if (image == NULL)
	{
		printf("Error allocating space for image");
		exit(0);
	}
	
	if (!ImageLoad(image_location, image))
	{
		exit(1);
	}
	
	return image;
}

//Membuat Method Resize
void Resize(int width, int height)
{
	glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (float)width/(float)height, 1.0, 300.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void TimeOut(int id)
{
    glutPostRedisplay();
    glutTimerFunc(1000/60, TimeOut, 0);
}

void PressSpecialKeys(int key, int x, int y)
{
	switch (key)
	{
		case GLUT_KEY_LEFT:
			direction = LEFT;
			break;
		case GLUT_KEY_RIGHT:
			direction = RIGHT;
			break;
		case GLUT_KEY_DOWN:
			direction = BACK;
			break;
		case GLUT_KEY_UP:
			direction = FRONT;
			break;
	}
}

void ReleaseSpecialKeys(int key, int x, int y)
{
	switch (key)
	{
		case GLUT_KEY_LEFT:
		case GLUT_KEY_RIGHT:
		case GLUT_KEY_DOWN:
		case GLUT_KEY_UP:
			direction = NONE;
			break;
	}
}

void PressNormalKey(unsigned char key, int x, int y)
{   
	switch (key) 
	{
		case 27:
			exit(1);
			break;
		case 'w':
			direction = ROT_UP;
			break;
		case 's':
			direction = ROT_DOWN;
			break;
		case 'd':
			direction = ROT_RIGHT;
			break;
		case 'a':
			direction = ROT_LEFT;
			break;
	}
}

void ReleaseNormalKey(unsigned char key, int x, int y)
{
	switch(key) {
		case 'w':
		case 's':
		case 'd':
		case 'a':
			direction = NONE;
			break;	
	}
}

void ComputePosition()
{
	glLoadIdentity();
	
	switch(direction) {
		case FRONT:
			z_pos += 0.1f;
			break;
		case BACK:
			z_pos -= 0.1f;
			break;
		case LEFT:
			xx += 0.1;
			break;
		case RIGHT:
			xx -= 0.1;
			break;
	}
	
	glTranslatef(xx,yy,z_pos);
	glRotatef(xpos, 1, 0, 0);
	glRotatef(rot, 0, 1, 0);
	glRotatef(zpos, 0, 0, 1);
}

void SkyBox()
{
	// Langit Belakang & Kanan
	glPushMatrix();
		glColor3f(1.0f, 1.0f, 1.0f);
		glBindTexture(GL_TEXTURE_2D, texture[0]);
		glBegin(GL_QUADS);
			flatCube(-80, 80, -80, 80, -4, 80);
		glEnd();
	glPopMatrix();
}

void Draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	ComputePosition();

	// Ground
	glPushMatrix();
		glBindTexture(GL_TEXTURE_2D, texture[1]);
		glColor3f(1,1,1);
		glTranslatef(-20.0f, 0.0f, -10.0f);
		glBegin(GL_QUADS);
			flatCube(-79.0f, -3.0f, -79.0f, 79.0f, -3.0f, 79.0f);
		glEnd();
	glPopMatrix();
	
	SkyBox();
//	printf("Dir: %i\n", direction);
	
	glutSwapBuffers();
}

// Membuat Method inisialisasi
void Init()
{
	glEnable(GL_TEXTURE_2D);
	glShadeModel(GL_FLAT);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glGenTextures(35, texture);
	
	Image* skybox_back 	= LoadTexture("Asset/Black.bmp");
	Image* grass		= LoadTexture("Asset/Green_Grass.bmp");
	
	// ============================
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, skybox_back->sizeX, skybox_back->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, skybox_back->data);
	
	// ============================
	glBindTexture(GL_TEXTURE_2D, texture[1]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, grass->sizeX, grass->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, grass->data);
}

int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);
   glutInitWindowSize(1500,600);
   glutInitWindowPosition(0,0);
   glutCreateWindow("Gedung H");
   Init();
   
   glutDisplayFunc(Draw);
   glutReshapeFunc(Resize);
   glutKeyboardFunc(PressNormalKey);
   glutKeyboardUpFunc(ReleaseNormalKey);
   glutSpecialFunc(PressSpecialKeys);
   glutSpecialUpFunc(ReleaseSpecialKeys);
   glutTimerFunc(0, TimeOut, 0);
   
   glutMainLoop();
   
   return 0;
}
