
#include <stdlib.h>
#include <GL/glut.h>
#include <vector>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

//#define STB_IMAGE_IMPLEMENTATION
//#define WIN32
//static GLfloat spin = 0.0;


void findMaxAndMin(GLfloat *arrayfloat);
void findScale(GLfloat maxX, GLfloat maxY, GLfloat maxZ);
void changeLight();

std::vector<GLfloat*> vertices;
std::vector<GLfloat*> vnormals;
std::vector<GLint*> faces;
std::vector<GLfloat*> fnormals;

int n;
int m;
int z;

GLfloat maxX;
GLfloat minX;
GLfloat maxY;
GLfloat minY;
GLfloat maxZ;
GLfloat minZ;

GLfloat centerX;
GLfloat centerY;
GLfloat centerZ;

GLfloat scale;


GLfloat vRoation;
GLfloat hRoation;

bool isPressed;
bool first = true;
GLfloat oldX;
GLfloat oldY;

double fov;
double aspect;
double zNear;
double zFar;

GLfloat diffuse0[] = { 1.0, 0.0, 0.0, 1.0 };
GLfloat ambient0[] = { 1.0, 0.0, 0.0, 1.0 };
GLfloat specular0[] = { 1.0, 0.0, 0.0, 1.0 };
GLfloat light0_pos[] = { -3.0, 2.0, 3.0, 1.0 };

GLfloat diffuse1[] = { 0.0, 0.0, 1.0, 1.0 };
GLfloat ambient1[] = { 0.0, 0.0, 1.0, 1.0 };
GLfloat specular1[] = { 0.0, 0.0, 1.0, 1.0 };
GLfloat light1_pos[] = { 3.0, 2.0, 3.0, 1.0 };

GLfloat ambient[] = { 0.2, 0.2, 0.2, 1.0 };
GLfloat diffuse[] = { 1.0, 0.8, 0.0, 1.0 };
GLfloat specular[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat shine = 100.0;
GLfloat light_ambient[] = { -1, -1, -1, 0.5 };




//opens the file specified (cube.obj is default) and grabs the face and vertices data
void openFile()
{
	maxX = NULL;
	minX = NULL;
	maxY = NULL;
	minY = NULL;
	maxZ = NULL;
	minZ = NULL;

	char *file = "cube.obj";

	switch (m)
	{
	case (1):
		file = "cube.obj";
		break;
	case (2):
		file = "pig.obj";
		break;
	case(3):
		file = "teapot.obj";
		break;
	default:
		break;
	}
	FILE *input;
	char c;
	GLfloat f1, f2, f3, *arrayfloat;
	GLint d1, d2, d3, *arrayint;
	vertices.clear();
	faces.clear();
	fnormals.clear();
	vnormals.clear();
	input = fopen(file, "r");

	while (!feof(input)) {
		fscanf(input, "%c", &c);
		if (c == 'v') {
			arrayfloat = new GLfloat[3];
			fscanf(input, "%f %f %f", &f1, &f2, &f3);
			arrayfloat[0] = f1;
			arrayfloat[1] = f2;
			arrayfloat[2] = f3;
			vertices.push_back(arrayfloat);
			findMaxAndMin(arrayfloat);
		}
		else if (c == 'f') {
			arrayint = new GLint[3];
			fscanf(input, "%d %d %d", &d1, &d2, &d3);
			arrayint[0] = d1;
			arrayint[1] = d2;
			arrayint[2] = d3;
			faces.push_back(arrayint);
		}
	}
	fclose(input);

	if (m == 1)
	{
		int x;
		int y;
		int n;
		//unsigned char* image_data = stbi_load("checker.jpg", &x, &y, &n, STBI_rgb_alpha);

		if (z == 0)
		{
			unsigned char* image_data = stbi_load("checker.jpg", &x, &y, &n, STBI_rgb_alpha);

			static GLuint texName;
			glEnable(GL_TEXTURE_2D);
			glGenTextures(1, &texName);
			glBindTexture(GL_TEXTURE_2D, texName);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			// select modulate to mix texture with color for shading
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, x,
				y, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
		}
		else if (z == 1)
		{
			unsigned char* image_data = stbi_load("mandrill.jpg", &x, &y, &n, STBI_rgb_alpha);

			static GLuint texName;
			glEnable(GL_TEXTURE_2D);
			glGenTextures(1, &texName);
			glBindTexture(GL_TEXTURE_2D, texName);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			// select modulate to mix texture with color for shading
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, x,
				y, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
		}
	}
	centerX = (maxX + minX) / 2;
	centerY = (maxY + minY) / 2;
	centerZ = (maxZ + minZ) / 2;

	findScale(maxX, maxY, maxZ);

	//start of new stuff
	GLfloat Ux, Uy, Uz, Vx, Vy, Vz, *floats, magnitude;
	for (int i = 0; i < faces.size(); i++) {
		floats = new GLfloat[3];
		Ux = vertices[faces[i][1] - 1][0] - vertices[faces[i][0] - 1][0];
		Uy = vertices[faces[i][1] - 1][1] - vertices[faces[i][0] - 1][1];
		Uz = vertices[faces[i][1] - 1][2] - vertices[faces[i][0] - 1][2];
		Vx = vertices[faces[i][2] - 1][0] - vertices[faces[i][0] - 1][0];
		Vy = vertices[faces[i][2] - 1][1] - vertices[faces[i][0] - 1][1];
		Vz = vertices[faces[i][2] - 1][2] - vertices[faces[i][0] - 1][2];
		floats[0] = Uy*Vz - Uz*Vy;
		floats[1] = Uz*Vx - Ux*Vz;
		floats[2] = Ux*Vy - Uy*Vx;
		magnitude = sqrt(floats[0] * floats[0] + floats[1] * floats[1] + floats[2] * floats[2]);
		floats[0] /= magnitude;
		floats[1] /= magnitude;
		floats[2] /= magnitude;
		fnormals.push_back(floats);
	}
	std::vector<GLint> adj;
	for (int i = 0; i < vertices.size(); i++)
	{
		//std::vector<GLint> adj;
		for (int j = 0; j < faces.size(); j++)
		{
			if (faces[j][0] - 1 == i)
			{
				adj.push_back(j);
			}
			else if (faces[j][1] - 1 == i)
			{
				adj.push_back(j);
			}
			else if (faces[j][2] - 1 == i)
			{
				adj.push_back(j);
			}
			
		}
		floats = new GLfloat[3];
		floats[0] = 0;
		floats[1] = 0;
		floats[2] = 0;
		for (int j = 0; j < adj.size(); j++)
		{
			floats[0] += fnormals[adj[j]][0];
			floats[1] += fnormals[adj[j]][1];
			floats[2] += fnormals[adj[j]][2];
		}

		magnitude = sqrt(floats[0] * floats[0] + floats[1] * floats[1] + floats[2] * floats[2]);
		floats[0] /= magnitude;
		floats[1] /= magnitude;
		floats[2] /= magnitude;
		vnormals.push_back(floats);
	}
	first = true;
	glLoadIdentity();

	changeLight();

}

void changeLight()
{
	glMatrixMode(GL_PROJECTION);
	glMatrixMode(GL_MODELVIEW);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	//glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glLightfv(GL_LIGHT0, GL_POSITION, light0_pos);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient0);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse0);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular0);
	glLightfv(GL_LIGHT1, GL_POSITION, light1_pos);
	glLightfv(GL_LIGHT1, GL_AMBIENT, ambient1);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse1);
	glLightfv(GL_LIGHT1, GL_SPECULAR, specular1);
	glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
	glMaterialf(GL_FRONT, GL_SHININESS, shine);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, light_ambient);
	glShadeModel(GL_SMOOTH);
}

//finds which diminsions max value minus center value will be used as the scale
//the largest of those calcutions will be used as the scale.
void findScale(GLfloat maxX, GLfloat maxY, GLfloat maxZ)
{
	GLfloat x = maxX - centerX;
	GLfloat y = maxY - centerY;
	GLfloat z = maxZ - centerZ;

	scale = 1/x;
	if (x < y)
		scale = 1/y;
	if (y < z)
		scale = 1/z;
}

//finds both the max and min values of x, y, z
//to calculate the center diminsions and scale
void findMaxAndMin(GLfloat *arrayfloat)
{
	if (minX == NULL)
	{
		minX = arrayfloat[0];
		maxX = arrayfloat[0];
		minY = arrayfloat[1];
		maxY = arrayfloat[1];
		minZ = arrayfloat[2];
		maxZ = arrayfloat[2];
		return;
	}

	if (minX > arrayfloat[0])
	{
		minX = arrayfloat[0];
	}
	if (maxX < arrayfloat[0])
	{
		maxX = arrayfloat[0];
	}

	if (minY > arrayfloat[1])
	{
		minY = arrayfloat[1];
	}
	if (maxY < arrayfloat[1])
	{
		maxY = arrayfloat[1];
	}

	if (minZ > arrayfloat[2])
	{
		minZ = arrayfloat[2];
	}
	if (maxZ < arrayfloat[2])
	{
		maxZ = arrayfloat[2];
	}

	return;
}

void display(void)
{

   glPushMatrix();
   //glClearColor(1,1,1,1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor3f(1.0, 1.0, 1.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fov, aspect, zNear, zFar);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0, 0, 5, 0, 0, 0, 0, 1, 0);
	//rotates the objects
	glRotatef(vRoation, 1.0, 0.0, 0.0);
	glRotatef(hRoation, 0.0, 0.0, 1.0);
	//scales down the objects to have them fit in the frame
	glScalef(scale, scale, scale);
	//puts the object in the frame so the user can see it
	glTranslatef(centerX * (-1), centerY * (-1), centerZ * (-1));

	switch(n)
	{
		//displays object as points
		case 1:
			glBegin(GL_POINTS);
			for (int i = 0; i < vertices.size(); i++)
			{
				glVertex3fv(vertices[i]);
			}
			glEnd();
			break;
		//displays object as lines
		case 2:
			glBegin(GL_LINES);
			for (int i = 0; i < faces.size(); i++)
			{
				glVertex3fv(vertices[faces[i][0] - 1]);
				glVertex3fv(vertices[faces[i][1] - 1]);
				glVertex3fv(vertices[faces[i][1] - 1]);
				glVertex3fv(vertices[faces[i][2] - 1]);
				glVertex3fv(vertices[faces[i][2] - 1]);
				glVertex3fv(vertices[faces[i][0] - 1]);

			}
			glEnd();
			break;
		//displays object as surface
		case 3:
			glBegin(GL_TRIANGLES);
			for (int i = 0; i<faces.size(); i++)
			{
				//glColor3f(1.0, 0.0, 1.0);
				glNormal3fv(vnormals[faces[i][0] -1]);
				glVertex3fv(vertices[faces[i][0] - 1]);
				glNormal3fv(vnormals[faces[i][1] - 1]);
				glVertex3fv(vertices[faces[i][1] - 1]);
				glNormal3fv(vnormals[faces[i][2] - 1]);
				glVertex3fv(vertices[faces[i][2] - 1]);
			}
			glEnd();
			break;
		case 4:
			glBegin(GL_TRIANGLES);
			for (int i = 0; i < faces.size(); i++) {
				glNormal3fv(vnormals[faces[i][0] - 1]);
				glNormal3fv(vnormals[faces[i][1] - 1]);
				glNormal3fv(vnormals[faces[i][2] - 1]);
				if (i != 8 && i != 9 && i != 10 && i != 11) {
					for (int j = 0; j < 3; j++) {
						int num = vertices[faces[i][j] - 1][2] + vertices[faces[i][j] - 1][1] + vertices[faces[i][j] - 1][0];
						if (num == 0) {
							glTexCoord2f(0, 0);
						}
						else if (num == -2) {
							glTexCoord2f(0, 1);
						}
						else if (num == -4) {
							glTexCoord2f(1, 1);
						}
						else {
							glTexCoord2f(1, 0);
						}
						glVertex3fv(vertices[faces[i][j] - 1]);
					}
				}
				else if (i == 9 || i == 11) {
					glTexCoord2f(0, 0);
					glVertex3fv(vertices[faces[i][0] - 1]);
					glTexCoord2f(1, 0);
					glVertex3fv(vertices[faces[i][1] - 1]);
					glTexCoord2f(1, 1);
					glVertex3fv(vertices[faces[i][2] - 1]);
				}
				else if (i == 10) {
					glTexCoord2f(1, 0);
					glVertex3fv(vertices[faces[i][0] - 1]);
					glTexCoord2f(0, 0);
					glVertex3fv(vertices[faces[i][1] - 1]);
					glTexCoord2f(1, 1);
					glVertex3fv(vertices[faces[i][2] - 1]);
				}
				else if (i == 8) {
					glTexCoord2f(1, 1);
					glVertex3fv(vertices[faces[i][0] - 1]);
					glTexCoord2f(0, 0);
					glVertex3fv(vertices[faces[i][1] - 1]);
					glTexCoord2f(1, 0);
					glVertex3fv(vertices[faces[i][2] - 1]);
				}
			}
			glEnd();
			break;

		default: break;

	}
	glPopMatrix();
	glutSwapBuffers();
}


void mouse(int button, int state, int x, int y)
{
	switch (button) {
		case GLUT_LEFT_BUTTON:
			if (state == GLUT_DOWN)
			{
				oldX = x;
			oldY = y;
			isPressed = true;
			}
		break;
		default:
			isPressed = false;
			break;
	}
}


void drag(int x, int y) {
	if (isPressed)
	{
		hRoation += 0.01*(x - oldX);
		vRoation += 0.01*(y - oldY);
	}
	glutPostRedisplay();
}


void init(void) 
{
   glClearColor (0.0, 0.0, 0.0, 0.0);
   //glShadeModel (GL_FLAT);
   glShadeModel(GL_SMOOTH);
}


void reshape(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//he used 1000 instead of 100
	fov = 45;
	aspect = w / h;
	zNear = 0.1;
	zFar = 100;
	gluPerspective(fov, aspect, zNear, zFar);
	//could have put look at in display
	//he used -2 instead of 10
	//gluLookAt(0, 0, -1, 0, 0, 0, 0, 1, 0);
	//gluLookAt(0,0,10,0,0,0,0,1,0);
	//if (w <= h)
	//	glOrtho(-2.0, 2.0, -2.0 * (GLfloat)h / (GLfloat)w,
	//		2.0 * (GLfloat)h / (GLfloat)w, -10.0, 10.0);
	//else
	//	glOrtho(-2.0 * (GLfloat)w / (GLfloat)h,
	//		2.0 * (GLfloat)w / (GLfloat)h, -2.0, 2.0, -10.0, 10.0);
	glMatrixMode(GL_MODELVIEW);
	glutPostRedisplay();
}

void reset()
{
	fov = 45;
	aspect = 1;
	zNear = 0.1;
	//was 100
	zFar = 100;

	ambient[0] = 0.2; ambient[1] = 0.2; ambient[2] = 0.2; ambient[3] = 1.0;
	diffuse[0] = 1.0; diffuse[1] = 0.8; diffuse[2] = 0.0; diffuse[3] = 1.0;
	specular[0] =  1.0; specular[1] = 1.0; specular[2] = 1.0; specular[3] = 1.0;
}

//switch statement to decide when a key is pressed
//to load and display a new object or change the display type
void key(unsigned char k, int x, int y)
{

	//need to add translation to this
	switch (k)
	{
	case 'a': 
		n = 1;
		display();
		break;
	case 's':
		n = 2;
		display();
		break;
	case 'd' :
		n = 3;
		display();
		break;
	case 'f':
		n = 4;
		display();
		break;
	case 'q':
		m = 1;
		vRoation = -90;
		hRoation = 90;
		openFile();
		display();
		break;
	case 'w':
		m = 2;
		vRoation = -90;
		hRoation = 90;
		openFile();
		display();
		break;
	case 'e':
		m = 3;
		vRoation = -90;
		hRoation = 90;
		openFile();
		display();
		break;
	case 'z':
		scale = scale + .01;
		display();
		break;
	case 'x':
		scale = scale - .01;
		if (scale < 0)
		{
			scale = 0.0;
		}
		display();
		break;
	//case 'i':
	//	centerX = centerX - 0.1;
	//	display();
	//	break;
	//case 'o':
	//	centerX = centerX + 0.1;
	//	display();
	//	break;
//	case 'j':
//		centerY = centerY - 0.1;
//		display();
//		break;
//	case 'k':
//		centerY = centerY + 0.1;
//		display();
//		break;
	//case 'n':
	//	centerZ = centerZ - 0.1;
	//	display();
	//	break;
	//case 'm':
	//	centerZ = centerZ + 0.1;
	//	display();
	//	break;
	case 'i':
		diffuse0[0] -= .1; diffuse0[1] -= .1; diffuse0[2] -= .1; diffuse0[3] -= .1;
		ambient0[0] -= .1; ambient0[1] -= .1; ambient0[2] -= .1; ambient0[3] -= .1;
		specular0[0] -= .1; specular0[1] -= .1; specular0[2] -= .1; specular0[3] -= .1;
		diffuse1[0] -= .1; diffuse1[1] -= .1; diffuse1[2] -= .1; diffuse1[3] -= .1;
		ambient1[0] -= .1; ambient1[1] -= .1; ambient1[2] -= .1; ambient1[3] -= .1;
		specular1[0] -= .1; specular1[1] -= .1; specular1[2] -= .1; specular1[3] -= .1;
		changeLight();
		//openFile();
		//display();
		glutPostRedisplay();
		break;
	case 'o':
		diffuse0[0] += .1; diffuse0[1] += .1; diffuse0[2] += .1; diffuse0[3] += .1;
		ambient0[0] += .1; ambient0[1] += .1; ambient0[2] += .1; ambient0[3] += .1;
		specular0[0] += .1; specular0[1] += .1; specular0[2] += .1; specular0[3] += .1;
		diffuse1[0] += .1; diffuse1[1] += .1; diffuse1[2] += .1; diffuse1[3] += .1;
		ambient1[0] += .1; ambient1[1] += .1; ambient1[2] += .1; ambient1[3] += .1;
		specular1[0] += .1; specular1[1] += .1; specular1[2] += .1; specular1[3] += .1;
		changeLight();
		glutPostRedisplay();
		break;
	case 'j':
		diffuse[0] -= .1; diffuse[1] -= .1; diffuse[2] -= .1; diffuse[3] -= .1;
		ambient[0] -= .1; ambient[1] -= .1; ambient[2] -= .1; ambient[3] -= .1;
		specular[0] -= .1; specular[1] -= .1; specular[2] -= .1; specular[3] -= .1;
		changeLight();
		display();
		//glutPostRedisplay();
		break;
	case 'k':
		diffuse[0] += .1; diffuse[1] += .1; diffuse[2] += .1; diffuse[3] += .1;
		ambient[0] += .1; ambient[1] += .1; ambient[2] += .1; ambient[3] += .1;
		specular[0] += .1; specular[1] += .1; specular[2] += .1; specular[3] += .1;
		changeLight();
		//openFile();
		display();
		//glutPostRedisplay();
		break;
	case 'n':
		light_ambient[0] -= 0.1;
		light_ambient[1] -= 0.1;
		light_ambient[2] -= 0.1;
		light_ambient[3] -= 0.1;
		changeLight();
		//openFile();
		display();
		break;
	case 'm':
		light_ambient[0] += 0.1;
		light_ambient[1] += 0.1;
		light_ambient[2] += 0.1;
		light_ambient[3] += 0.1;
		//openFile();
		changeLight();
		display();
		break;
	case 'r':
		vRoation = vRoation - 5.0;
		display();
		break;
	case 't':
		vRoation = vRoation + 5.0;
		display();
		break;
	//case 'f':
	//	hRoation = hRoation - 5.0;
	//	display();
	//	break;
	case 'g':
		z = 1;
		openFile();
		display();
		break;
		//added these two because I though it would be fun
	case '1':
		glDisable(GL_LIGHT0);
		glutPostRedisplay();
		break;
	case '2':
		glDisable(GL_LIGHT1);
		glutPostRedisplay();
		break;
	case '3':
		glEnable(GL_LIGHT0);
		glutPostRedisplay();
		break;
	case '4':
		glEnable(GL_LIGHT1);
		glutPostRedisplay();
		break;
	case '5':
		fov += 2;
		display();
		break;
	case '6':
		fov -= 2;
		display();
		break;
	case '7':
		aspect += 2;
		display();
		break;
	case '8':
		aspect -= 2;
		display();
		break;
	case '9':
		reset();
		display();
		break;
	case '0':
		zNear += 2;
		display();
		break;
	case '-':
		zNear -= 2;
		display();
		break;
	case '[':
		zFar += 10;
		display();
		break;
	case ']':
		zFar -= 10;
		display();
		break;
	case 'h':
		z = 0;
		openFile();
		display();
		break;
	case 'b':
		glShadeModel(GL_SMOOTH);
		glutPostRedisplay();
		break;
	case ',':
		glDisable(GL_LIGHTING);
		glutPostRedisplay();
		break;
	case '.':
		glEnable(GL_LIGHTING);
		glutPostRedisplay();
		break;
	default: break;
	
	}
}

   
/* 
 *  Request double buffer display mode.
 *  Register mouse input callback functions
 */
int main(int argc, char** argv)
{
	//default values
	//cube object
	//dispalyed as lines
	m = 1;
	n = 2;
	z = 0;

	vRoation = -90;
	hRoation = 90;

   glutInit(&argc, argv);
   //added glut depth
   glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
   glutInitWindowSize (500, 500); 
   glutInitWindowPosition (100, 100);
   glutCreateWindow (argv[0]);
   init ();
   openFile();
   glutDisplayFunc(display); 
   glutReshapeFunc(reshape); 

   glutMouseFunc(mouse);
   glutMotionFunc(drag);
   glutKeyboardUpFunc(key);
   glutMainLoop();
   return 0;   /* ANSI C requires main to return int. */
}