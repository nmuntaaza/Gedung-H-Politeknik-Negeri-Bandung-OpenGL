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

Image* loadTexture(char* image_location)
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

//Membuat Method resize
void resize(int width, int height)
{
	glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (float)width/(float)height, 1.0, 300.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

//Membuat Method timeout
void myTimeOut(int id)
{
	// called if timer event
	// ...advance the state of animation incrementally...
	// rot+=10;
    glutPostRedisplay(); // request redisplay
    glutTimerFunc(100, myTimeOut, 0); // request next timer event
}

void PressSpecialKeys(int key, int x, int y)
{
	switch (key)
	{
		case GLUT_KEY_LEFT:
			xx += 0.1;
			glutPostRedisplay();
			break;
		case GLUT_KEY_RIGHT:
			xx -= 0.1;
			glutPostRedisplay();
			break;
		case GLUT_KEY_DOWN:
			z_pos -= 0.1f;
			glutPostRedisplay();
			break;
		case GLUT_KEY_UP:
			z_pos += 0.1f;
			glutPostRedisplay();
			break;
	}
}

void myKeyboard(unsigned char key,int x, int y)
{
	float fraction = 0.1f;
    
	switch (key) 
	{
		case 'h':
			xpos = xpos + 5;
			glutPostRedisplay();
			break;
		case 'j':
			xpos = xpos - 5;
			glutPostRedisplay();
		case 's':
			z_pos -= 0.1f;
			glutPostRedisplay();
			break;
		case 'w':
			z_pos += 0.1f;
			glutPostRedisplay();
			break;
		case 'k' :
			rot += 10;
			glutPostRedisplay();
			break;
		case 'l' :
			rot -= 10;
			glutPostRedisplay();
			break;
		case 'd':
			xx -= 0.1;
			glutPostRedisplay();
			break;
		case 'a':
			xx += 0.1;
			glutPostRedisplay();
			break;
	}
}

void SkyBox()
{
	// Langit Belakang & Kanan
	glPushMatrix();
		glColor3f(1.0f, 1.0f, 1.0f);
		glBindTexture(GL_TEXTURE_2D, texture[6]);
		glBegin(GL_QUADS);
			flatCube(-80, 80, -80, 80, -10, -80);
		glEnd();
	glPopMatrix();
}

// Membuat Method display
void mydisplay()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glTranslatef(xx,yy,z_pos);
	
	glRotatef(xpos, 1, 0, 0);
	glRotatef(rot, 0, 1, 0);
	glRotatef(zpos, 0, 0, 1);

	// Tanah Basement
	glPushMatrix();
		glBindTexture(GL_TEXTURE_2D, texture[2]);
		glColor3f(1,1,1);
		glTranslatef(-20.0f, 0.0f, -10.0f);
		glBegin(GL_QUADS);
			flatCube(15.0f, -3.0f, 3.0f, 20.0f, -3.0f, -15.0f);
		glEnd();
	glPopMatrix();
	
	SkyBox();
	
	glutSwapBuffers();
}

// Membuat Method inisialisasi
void init()
{
	glEnable(GL_TEXTURE_2D);
	glShadeModel(GL_FLAT);
	glClearColor(0.5, 0.5, 0.5, 0.0);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glGenTextures(35, texture);
	
	Image* skybox_back 	= loadTexture("Asset/Daylight_Box_Back.bmp");
	Image* skybox_front 	= loadTexture("Asset/Daylight_Box_Front.bmp");
	Image* skybox_top 	= loadTexture("Asset/Daylight_Box_Top.bmp");
	Image* skybox_bottom 	= loadTexture("Asset/Daylight_Box_Bottom.bmp");
	Image* skybox_left 	= loadTexture("Asset/Daylight_Box_Left.bmp");
	Image* skybox_right 	= loadTexture("Asset/Daylight_Box_Right.bmp");
   
	if (skybox_back == NULL) 
	{
		printf("Image was not returned from loadTexture\n");
		exit(0);
	}

	if (skybox_front == NULL) 
	{
		printf("Image was not returned from loadTexture\n");
		exit(0);
	}
	
	if (skybox_top == NULL) 
	{
		printf("Image was not returned from loadTexture\n");
		exit(0);
	}
	
	if (skybox_bottom == NULL) 
	{
		printf("Image was not returned from loadTexture\n");
		exit(0);
	}
	
	// ============================
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	// Menyesuaikan ukuran textur ketika image lebih besar dari texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	// Menyesuaikan ukuran textur ketika image lebih kecil dari texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	
	glTexImage2D(GL_TEXTURE_2D, 0, 3, skybox_back->sizeX, skybox_back->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, skybox_back->data);
	
	// ============================
	glBindTexture(GL_TEXTURE_2D, texture[1]);
	// Menyesuaikan ukuran textur ketika image lebih besar dari texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	// Menyesuaikan ukuran textur ketika image lebih kecil dari texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	
	glTexImage2D(GL_TEXTURE_2D, 0, 3, skybox_front->sizeX, skybox_front->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, skybox_front->data);
	
	// ============================
	glBindTexture(GL_TEXTURE_2D, texture[2]);
	// Menyesuaikan ukuran textur ketika image lebih besar dari texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	// Menyesuaikan ukuran textur ketika image lebih kecil dari texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	
	glTexImage2D(GL_TEXTURE_2D, 0, 3, skybox_top->sizeX, skybox_top->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, skybox_top->data);
	
	// ============================
	glBindTexture(GL_TEXTURE_2D, texture[3]);
	// Menyesuaikan ukuran textur ketika image lebih besar dari texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	// Menyesuaikan ukuran textur ketika image lebih kecil dari texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	
	glTexImage2D(GL_TEXTURE_2D, 0, 3, skybox_bottom->sizeX, skybox_bottom->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, skybox_bottom->data);
}

int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);
   glutInitWindowSize(1500,600);
   glutInitWindowPosition(0,0);
   glutCreateWindow("Gedung H");
   
   glutDisplayFunc(mydisplay);
   glutKeyboardFunc(myKeyboard);
   glutSpecialFunc(PressSpecialKeys);
   glutTimerFunc(100, myTimeOut, 0);
   glutReshapeFunc(resize);
   init();
   
   glutMainLoop();
   return 0;
}
