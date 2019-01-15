#include <stdio.h>
#include <cmath>
#include <GL/glut.h>

void Init();
void Draw();
void ReDraw(int width, int height);
void Timer(int);
void ProcessNormalKeys(unsigned char key, int x, int y);
void ProcessSpecialKeys(int key, int x, int y);
void DrawSnowMan();
void ReleaseSpecialKey(int key, int x, int y);
void PressSpecialKey(int key, int x, int y);
void ReleaseNormalKey(unsigned char key, int x, int y);
void PressNormalKey(unsigned char key, int x, int y);
void ComputePos(float delta_move);
void ComputeDir(float delta_angle);
void Debug();

float red = 0.0f, green = 0.0f, blue = 1.0f;
// Angle of rotation for the camera direction
float angle = 0.0f;
// Actual vector representing the camera's direction
float lx = 0.0f, ly = 0.0f, lz = -1.0f;
// Camera position
float x = 0.0f, y = 1.0f, z = 5.0f;
// Handle translation
float xx = 0.0f, yy = 0.0f, zz = 0.0f;
// The key states. These variables will be zero when no key is being presses
float delta_angle = 0.0f, delta_move = 0.0f;

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(500, 500);
	glutCreateWindow("Gedung H");
	Init();
	
	glutDisplayFunc(Draw);
	glutReshapeFunc(ReDraw);
	glutIdleFunc(Draw);
	
	glutKeyboardFunc(PressNormalKey);
	glutSpecialFunc(PressSpecialKey);
	
	glutKeyboardUpFunc(ReleaseNormalKey);
	glutSpecialUpFunc(ReleaseSpecialKey);
	
	glEnable(GL_DEPTH_TEST);
	
	glutMainLoop();
	return 0;
}

void Init()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glutIgnoreKeyRepeat(1);
}

void Draw()
{
	Debug();
	
	if (delta_move) 
	{
		ComputePos(delta_move);
	}
	if (delta_angle)
	{
		ComputeDir(delta_angle);
	}
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	
	gluLookAt(
		x, 1.0f, z,
		x+lx, 1.5f, z+lz,
		0.0f, 1.0f, 0.0f
	);
	
	// Draw ground
	glColor3f(red, green, blue);
	glBegin(GL_QUADS);
		glVertex3f(-100.0f, 0.0f, -100.0f);
		glVertex3f(-100.0f, 0.0f,  100.0f);
		glVertex3f( 100.0f, 0.0f,  100.0f);
		glVertex3f( 100.0f, 0.0f, -100.0f);
	glEnd();
	
	for (int i = -3; i < 3; i++)
	{
		for (int j = -3; j < 3; j++)
		{
			glPushMatrix();
			glTranslatef(i * 10.0, 0, j * 10.0f);
			DrawSnowMan();
			glPopMatrix();
		}
	}
	
	glutSwapBuffers();
}

void ReDraw(int width, int height)
{
	if (width == 0) { width = 1; }
	if (height == 0) { height = 1; }
	float window_ratio = static_cast<float>(width) / static_cast<float>(height);
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, width, height);
	gluPerspective(45, window_ratio, 1, 1000);
	
	glMatrixMode(GL_MODELVIEW);
}

void Timer(int)
{
	glutPostRedisplay();
	glutTimerFunc(1000.0/60.0, Timer, 0);
	
	angle += 0.1f;
}

void ProcessNormalKeys(unsigned char key, int x, int y)
{
	// Exit if hit ESCAPE
	if (key == 27) { exit(0); }
	if (key == 'a') { xx += 0.5f; }
	if (key == 'd') { xx -= 0.5f; }
	if (key == 'w') { zz += 0.5f; }
	if (key == 's') { zz -= 0.5f; }
}

void ProcessSpecialKeys(int key, int x, int y)
{
	float fraction = 0.1f;
	
	switch (key)
	{
		case GLUT_KEY_LEFT:
			angle -= 0.1f;
			lx = sin(angle);
			lz = -cos(angle);
			break;
		case GLUT_KEY_RIGHT:
			angle += 0.1f;
			lx = sin(angle);
			lz = -cos(angle);
			break;
		case GLUT_KEY_UP:
			x += lx * fraction;
			z += lz * fraction;
			break;
		case GLUT_KEY_DOWN:
			x -= lx * fraction;
			z -= lz * fraction;
			break;
	}
}

void DrawSnowMan()
{
	glColor3f(1.0f, 1.0f, 1.0f);

	// Draw Body
	glTranslatef(0.0f ,0.75f, 0.0f);
	glutSolidSphere(0.75f,20,20);

	// Draw Head
	glTranslatef(0.0f, 1.0f, 0.0f);
	glutSolidSphere(0.25f,20,20);

	// Draw Eyes
	glPushMatrix();
	glColor3f(0.0f,0.0f,0.0f);
	glTranslatef(0.05f, 0.10f, 0.18f);
	glutSolidSphere(0.05f,10,10);
	glTranslatef(-0.1f, 0.0f, 0.0f);
	glutSolidSphere(0.05f,10,10);
	glPopMatrix();

	// Draw Nose
	glColor3f(1.0f, 0.5f , 0.5f);
	glutSolidCone(0.08f,0.5f,10,2);
}

void ComputePos(float delta_move)
{
	x += delta_move * lx * 0.1f;
	z += delta_move * lz * 0.1f;
}

void ComputeDir(float delta_angle)
{
	angle += delta_angle;
	lx = sin(angle);
	lz = -cos(angle);
}

void PressSpecialKey(int key, int x, int y)
{
	switch (key)
	{
		case GLUT_KEY_LEFT: delta_angle = -0.01f; break;
		case GLUT_KEY_RIGHT: delta_angle = 0.01f; break;
		case GLUT_KEY_UP: delta_move = 0.5f; break;
		case GLUT_KEY_DOWN: delta_move = -0.5f; break;
	}
}

void ReleaseSpecialKey(int key, int x, int y)
{
	switch (key)
	{
		case GLUT_KEY_LEFT:
		case GLUT_KEY_RIGHT: delta_angle = 0.0f; break;
		case GLUT_KEY_UP:
		case GLUT_KEY_DOWN: delta_move = 0.0f; break;
	}
}

void PressNormalKey(unsigned char key, int x, int y)
{
	switch (key)
	{
		case 'a':
			ly += 0.1f;
			break;
	}
}

void ReleaseNormalKey(unsigned char key, int x, int y)
{
	switch (key)
	{
		case 'a':
			ly = 1.0f;
			break;
	}
}

void Debug()
{
	printf("Angle: %.2f\n", angle);
	printf("LX: %.2f, LY: %.2f, LZ: %.2f\n", lx, ly, lz);
	printf("X: %.2f, Y: %.2f, Z: %.2f\n", x, y, z);
	printf("Delta Angle: %.2f, Delta Move: %.2f\n", delta_angle, delta_move);
	printf("================================\n");
}
