
#include <stdlib.h>
#include <GL/glut.h>
#include <vector>
//#define WIN32
//static GLfloat spin = 0.0;


void findMaxAndMin(GLfloat *arrayfloat);
void findScale(GLfloat maxX, GLfloat maxY, GLfloat maxZ);

std::vector<GLfloat*> vertices;
std::vector<GLint*> faces;

int n;
int m;

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
GLfloat oldX;
GLfloat oldY;




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


	centerX = (maxX + minX) / 2;
	centerY = (maxY + minY) / 2;
	centerZ = (maxZ + minZ) / 2;

	findScale(maxX, maxY, maxZ);

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

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor3f(1.0, 1.0, 1.0);

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
				glVertex3fv(vertices[faces[i][0] - 1]);
				glVertex3fv(vertices[faces[i][1] - 1]);
				glVertex3fv(vertices[faces[i][2] - 1]);
			}
			glEnd();

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
   glShadeModel (GL_FLAT);
}


void reshape(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//he used 1000 instead of 100
	gluPerspective(45, w / h, 0.1, 100);
	//could have put look at in display
	//he used -2 instead of 10
	gluLookAt(0,0,10,0,0,0,0,1,0);
	//if (w <= h)
	//	glOrtho(-2.0, 2.0, -2.0 * (GLfloat)h / (GLfloat)w,
	//		2.0 * (GLfloat)h / (GLfloat)w, -10.0, 10.0);
	//else
	//	glOrtho(-2.0 * (GLfloat)w / (GLfloat)h,
	//		2.0 * (GLfloat)w / (GLfloat)h, -2.0, 2.0, -10.0, 10.0);
	glMatrixMode(GL_MODELVIEW);
	glutPostRedisplay();
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
	case 'i':
		centerX = centerX - 0.1;
		display();
		break;
	case 'o':
		centerX = centerX + 0.1;
		display();
		break;
	case 'j':
		centerY = centerY - 0.1;
		display();
		break;
	case 'k':
		centerY = centerY + 0.1;
		display();
		break;
	case 'n':
		centerZ = centerZ - 0.1;
		display();
		break;
	case 'm':
		centerZ = centerZ + 0.1;
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
	case 'f':
		hRoation = hRoation - 5.0;
		display();
		break;
	case 'g':
		hRoation = hRoation + 5.0;
		display();
		break;
		//added these two because I though it would be fun
	case 'c':
		vRoation = vRoation - 5.0;
		hRoation = hRoation - 5.0; 
		display();
		break;
	case 'v':
		vRoation = vRoation + 5.0;
		hRoation = hRoation + 5.0;
		display();
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

	vRoation = -90;
	hRoation = 90;

   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
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