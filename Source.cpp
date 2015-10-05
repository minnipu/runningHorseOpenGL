//Mignon Kramp 30965134
// WINDOW ONLY
//#include <windows.h>					// included in all Windows apps
//#include <winuser.h>          // Windows constants
// for windows GL is in small letters
//#include <gl/gl.h>						// OpenGL include
//#include <gl/glu.h>
//#include <GL/openglut.h>
// for linux GL is in capital letters
//=========================================================//
//=========================================================//
// LINUX / Mac OSX or UNIX ONLY
#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>
//=========================================================//
//=========================================================//
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
//=========================================================//
//=========================================================//
// person position in the environment

void move_camera(int specialKey, char normalKey);
void update_camera();

GLdouble  g_playerPos[] = { 0.0, 1, 10.0 };//{ 0.0, 0.5, 10.0 };
GLdouble  g_lookAt[] = { 0.0, 0.0, 0.0 };
GLfloat   g_viewAngle = -90.0;
GLfloat   g_elevationAngle = 0.0;
float rad = 0;
const float DEFAULT_SPEED = 1.0f;
//=========================================================//
//=========================================================//
GLvoid  DrawGround();
GLvoid DrawHorse();
GLvoid DrawLinesXYZ();
float rotateBase_degrees = 0;
float rotateMult = 1;

//=========================================================//
//=========================================================//
// quadric objects
void init_data(void);
void setup_sceneEffects(void);
GLUquadricObj *g_normalObject = NULL;
GLUquadricObj *g_wireframeObject = NULL;
GLUquadricObj *g_flatshadedObject = NULL;
void cleanUP_data(void);

const int   WORLD_SIZE = 250;
//=========================================================//
//=========================================================//
static void text_onScreen(int row, int col, const char *fmt, ...);
//=========================================================//
//=========================================================//
static void text_onScreen(int row, int col, const char *fmt, ...)
{
	static char buf[256];
	int viewport[4];
	//void *font = GLUT_BITMAP_9_BY_15;
	va_list args;

	va_start(args, fmt);
	(void)vsprintf_s(buf, fmt, args);
	va_end(args);

	glGetIntegerv(GL_VIEWPORT, viewport);

	glPushMatrix();
	glLoadIdentity();

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	glOrtho(0, viewport[2], 0, viewport[3], -1, 1);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}
//=========================================================//
//=========================================================//
static void resize(int width, int height)
{
	const float ar = (float)width / (float)height;

	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (ar > .5)
		glFrustum(-ar, ar, -1.0, 1.0, 2.0, 100.0);
	else
		glFrustum(-1.0, 1.0, -1 / ar, 1 / ar, 2.0, 100.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}
//=========================================================//
//=========================================================//
GLvoid DrawGround()
{
	// enable blending for anti-aliased lines
	glEnable(GL_BLEND);

	// set the color to a bright blue
	glColor3f(0.5f, 0.7f, 1.0f);

	// draw the lines
	glBegin(GL_LINES);
	for (int x = -WORLD_SIZE; x < WORLD_SIZE; x += 6)
	{
		glVertex3i(x, 0, -WORLD_SIZE);
		glVertex3i(x, 0, WORLD_SIZE);
	}
	// set the color to a bright red
	glColor3f(1.0f, 0.7f, 0.5f);

	for (int z = -WORLD_SIZE; z < WORLD_SIZE; z += 6)
	{
		glVertex3i(-WORLD_SIZE, 0, z);
		glVertex3i(WORLD_SIZE, 0, z);
	}
	glEnd();

	// turn blending off
	glDisable(GL_BLEND);
} // end DrawGround()
  //=========================================================//
  //=========================================================//
static void display(void)
{
	update_camera();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_LIGHTING);

	// position the light
	GLfloat pos[4] = { 5.0, 5.0, 5.0, 0.0 };
	glLightfv(GL_LIGHT0, GL_POSITION, pos);

	// rotation is used for animation
	static GLfloat rotation = 0.0;
	// it's increased by one every frame
	rotation += 0.1*rotateMult;
	// and ranges between 0 and 360
	if (rotation > 360.0)
		rotation = 0.0;


	//glDisable(GL_LIGHTING);
	glColor3d(0.1, 0.1, 0.4);

	text_onScreen(0, 3, "Mignon Kramp / A385 Computer Graphics / UAA");
	text_onScreen(2, 3, "- look up or down: A / Z");
	text_onScreen(3, 3, "- look right/left: arrows ->/<-");
	text_onScreen(4, 3, "- walk forward/backward: arrows UP/Down");

	DrawGround();
	DrawHorse();
	DrawLinesXYZ();

	glutSwapBuffers();
}
//=========================================================//
//=========================================================//
static void keyboard(unsigned char key, int x, int y)
{
	int number = -1;

	move_camera(number, key);

	glutPostRedisplay();
}
//=========================================================//
//=========================================================//
void move_camera(int specialKEY, char normalKEY)
{

	// keyboard :: normal keys
	switch (normalKEY)
	{
	case 'R':
		rotateBase_degrees++;
		break;
	case 'r':
		rotateBase_degrees++;
		break;
	case 'L':
		rotateBase_degrees--;
		break;
	case 'l':
		rotateBase_degrees--;
		break;
	case '1':
		rotateMult = 1;
		break;
	case '2':
		rotateMult = 10;
		break;
	case '3':
		rotateMult = 100;
		break;
	default:
	{   break;
	}
	}
	// keyboard :: normal keys
	switch (normalKEY)
	{   // looking up
	case 'A':
	case 'a':    g_elevationAngle += 2.0; break;
		// looking down
	case 'Z':
	case 'z':g_elevationAngle -= 2.0;  break;

	default:
	{    break;
	}
	}

	// special :: special keys
	switch (specialKEY)
	{   // camera setup
		// check if it is moving the view to look left
	case GLUT_KEY_LEFT:
	{
		g_viewAngle -= 2.0;
		// calculate camera rotation angle radians
		rad = float(3.14159 * g_viewAngle / 180.0f);
		break;
	}
	// check if it is moving the view to look right
	case GLUT_KEY_RIGHT:
	{
		g_viewAngle += 2.0;
		// calculate camera rotation angle radians
		rad = float(3.14159 * g_viewAngle / 180.0f);
		break;
	}
	// pressing keys Up/Down, update coordinates "x" and "z"
	// based on speed and angle of view.
	case GLUT_KEY_UP:
	{
		g_playerPos[2] += sin(rad) * DEFAULT_SPEED;
		g_playerPos[0] += cos(rad) * DEFAULT_SPEED;
		break;
	}
	case GLUT_KEY_DOWN:
	{
		g_playerPos[2] -= sin(rad) * DEFAULT_SPEED;
		g_playerPos[0] -= cos(rad) * DEFAULT_SPEED;
		break;
	}
	default:
	{   break;
	}
	}
} // End move_camera()
  //=========================================================//
  //=========================================================//
void update_camera()
{
	// don't allow the player to wander past the "edge of the world"
	if (g_playerPos[0] < -(WORLD_SIZE - 50))
		g_playerPos[0] = -(WORLD_SIZE - 50);
	if (g_playerPos[0] > (WORLD_SIZE - 50))
		g_playerPos[0] = (WORLD_SIZE - 50);
	if (g_playerPos[2] < -(WORLD_SIZE - 50))
		g_playerPos[2] = -(WORLD_SIZE - 50);
	if (g_playerPos[2] > (WORLD_SIZE - 50))
		g_playerPos[2] = (WORLD_SIZE - 50);

	// calculate the player's angle of rotation in radians
	float rad = float(3.14159 * g_viewAngle / 180.0f);
	// use the players view angle to correctly set up the view matrix
	g_lookAt[0] = float(g_playerPos[0] + 100 * cos(rad));
	g_lookAt[2] = float(g_playerPos[2] + 100 * sin(rad));

	rad = float(3.13149 * g_elevationAngle / 180.0f);

	g_lookAt[1] = float(g_playerPos[1] + 100 * sin(rad));

	// clear the modelview matrix
	glLoadIdentity();

	// setup the view matrix
	gluLookAt(g_playerPos[0], g_playerPos[1], g_playerPos[2],
		g_lookAt[0], g_lookAt[1], g_lookAt[2],
		0.0, 1.0, 0.0);
}

//=========================================================//
//=========================================================//
static void special(int key, int x, int y)
{
	char letter = ' ';

	move_camera(key, letter);

	glutPostRedisplay();
}
  //=========================================================//
  //=========================================================//
static void idle(void)
{
	glutPostRedisplay();
}
  //=========================================================//
  //=========================================================//
void init_data(void)
{
	//setup_sceneEffects();
	// create a normal quadric (uses default settings)
	g_normalObject = gluNewQuadric();

	g_wireframeObject = gluNewQuadric();
	gluQuadricDrawStyle(g_wireframeObject, GLU_LINE);

	// create an object that uses flat shading
	g_flatshadedObject = gluNewQuadric();
	gluQuadricNormals(g_flatshadedObject, GLU_FLAT);

}
//=========================================================//
//=========================================================//
void cleanUP_data(void)
{  // delete all quadratic objects
	if (g_normalObject)
		gluDeleteQuadric(g_normalObject);
	if (g_wireframeObject)
		gluDeleteQuadric(g_wireframeObject);
	if (g_flatshadedObject)
		gluDeleteQuadric(g_flatshadedObject);
}
//=========================================================//
//=========================================================//
int main(int argc, char *argv[])
{
	glutInitWindowSize(640, 480);
	glutInitWindowPosition(40, 40);
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

	glutCreateWindow("A385 Computer Graphics");

	init_data();

	glutReshapeFunc(resize);
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(special);
	glutIdleFunc(idle);

	// environment background color
	glClearColor(0.9, 0.9, 0.7, 1);//(1,1,1,1);
								   // deepth efect to objects
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	// light and material in the environment
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);

	glutMainLoop();
	cleanUP_data();

	return EXIT_SUCCESS;
}
//------------------------------------------World Complete
//=========================================================//
//=========================================================//
//=========================================================//
GLvoid DrawLinesXYZ()
{
	glPushMatrix();
	
	glBegin(GL_LINES);
	glLineWidth(1.5);
	glColor3f(1.0, 0, 0);
	glVertex3f(0, 0, 0);
	glVertex3f(2.5, 0, 0);

	glColor3f(0, 1, 0);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 2.5, 0);

	glColor3f(0, 0, 1);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0, 2.5);

	glEnd();
	glPopMatrix();

}

GLvoid DrawHorse()
{
	//Body of Horse
	glPushMatrix();
	glTranslated(0, 1, 0);
	glRotated(90, 0, 1, 0);
	//DrawLinesXYZ();
	glColor3f(1.0, .50, .65);
	gluCylinder(g_normalObject, 0.35, 0.4, 1.4, 20, 8);
		glPushMatrix(); //make disk move relitive to cylinder
		gluDisk(g_normalObject, 0, 0.35, 20, 8);
		glTranslated(0, 0, 1.4);
		gluDisk(g_normalObject, 0, 0.4, 20, 8);
		glPopMatrix();
		
		//make neck bottom
		glPushMatrix();  
		glTranslatef(0, .4, 1.2);
		glRotated(-80, 1, 0, 0);
		gluCylinder(g_normalObject, 0.2, 0.19, 0.5, 20, 8);
		glPopMatrix();

		//make neck top
		glPushMatrix();
		glTranslatef(0, .9, 1.3);
		glRotated(-60, 1, 0, 0);
		gluCylinder(g_normalObject, 0.19, 0.10, 0.5, 20, 8);
		//DrawLinesXYZ();
		glPopMatrix();

		//make Head
		glPushMatrix();
		glTranslatef(0, 1.3, 1.7);
		glRotated(40, 1, 0, 0);
		gluCylinder(g_normalObject, 0.2, 0.1, 0.6, 20, 8);
		//DrawLinesXYZ();
		glPopMatrix();

		//make Horn
		glPushMatrix();
		glTranslatef(0, 1.2, 1.9);
		glRotated(-50, 1, 0, 0);
		gluCylinder(g_normalObject, 0.05, 0, 0.5, 20, 8);
		//DrawLinesXYZ();
		glPopMatrix();

		//make Leg Left Front
		glPushMatrix();
		glTranslatef(0.15, -.3, 1.3);
		glRotated(90, 1, 0, 0);
	    //DrawLinesXYZ();
		gluCylinder(g_normalObject, 0.1, 0.1, 0.6, 20, 8);
			//make Leg Left Front bottom
			glPushMatrix();
			glTranslatef(0, 0, 0.5);
			glRotated(10,10, 0, 0);
			gluCylinder(g_normalObject, 0.1, 0.1, 0.67, 20, 8);
			glPopMatrix();
		glPopMatrix();

		//make Leg Right Front
		glPushMatrix();
		glTranslatef(-.15, -.3, 1.3);
		glRotated(90, 1, 0, 0);
		gluCylinder(g_normalObject, 0.1, 0.1, 0.6, 20, 8);
			//make Leg Right Front bottom
			glPushMatrix();
			glTranslatef(0, 0, 0.5);
			glRotated(10, 10, 0, 0);
			gluCylinder(g_normalObject, 0.1, 0.1, 0.67, 20, 8);
			glPopMatrix();
		glPopMatrix();
		
		//make Leg Left Back
		glPushMatrix();
		glTranslatef(0.15, -.3, 0.15);
		glRotated(90, 1, 0, 0);
		gluCylinder(g_normalObject, 0.15, 0.1, 0.6, 20, 8);
			//make left leg back bottom
			glPushMatrix();
			glTranslatef(0, 0, 0.5);
			glRotated(10, -10, 0, 0);
			gluCylinder(g_normalObject, 0.1, 0.1, 0.67, 20, 8);
			glPopMatrix();
		//DrawLinesXYZ();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(-0.15, -.3, 0.15);
		glRotated(90, 1, 0, 0);
		gluCylinder(g_normalObject, 0.15, 0.1, 0.6, 20, 8);
			//make left leg back bottom
			glPushMatrix();
			glTranslatef(0, 0, 0.5);
			glRotated(10, -10, 0, 0);
			gluCylinder(g_normalObject, 0.1, 0.1, 0.67, 20, 8);
			glPopMatrix();
		glPopMatrix();

		//make tail
		glPushMatrix();
		glRotated(-50, 1, 0,0 );
		glTranslated(0, 0.07, -0.5);
		gluCylinder(g_normalObject, 0.18, 0.1, 0.6, 20, 8);
		glPopMatrix();

	
	glPopMatrix(); //End Horse
}
//------------------------------------------Horse Complete
//=========================================================//
//=========================================================//
GLvoid Trot()
{

}
