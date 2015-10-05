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
#include <iostream>
#include <time.h>
#include <string>
//=========================================================//
//=========================================================//
// person/horse position in the environment

void move_camera(int specialKey, char normalKey);
void update_camera();

GLdouble  g_playerPos[] = { 0.0, 1, 10.0 };//{ 0.0, 0.5, 10.0 };
GLdouble  g_lookAt[] = { 0.0, 0.0, 0.0 };
GLfloat   g_viewAngle = -90.0;
GLfloat   g_elevationAngle = 0.0;
float rad = 0;
static const float PI = 3.141592653;
const float DEFAULT_SPEED = 1.0f;
struct FrameAngles;
//=========================================================//
//=========================================================//
GLvoid DrawGround();
GLvoid DrawHorse(FrameAngles);
GLvoid UpdateHorse();
GLvoid DrawLinesXYZ();
float rotateBase_degrees = 0;
float horseSpeed = 1;

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
struct holdPosition //holds the positions on the object type position of object horse
{
	GLfloat x = 0.0;
	GLfloat y = 0.0;
	GLfloat z = 0.0;
	GLfloat angle = 90.0;
	GLfloat rad = PI*angle / 180; //converts angle to radians 
	bool initialized = false; 
	int state = 2;
} horse;

struct FrameAngles
{
	GLfloat neckbt;
	GLfloat necktp;
	GLfloat head;

	GLfloat frtLLegtp;
	GLfloat frtLLegbt;
	GLfloat frtRLegtp;
	GLfloat frtRlegbt;

	GLfloat bkLLegtp;
	GLfloat bkLLegbt;
	GLfloat bkRLegtp;
	GLfloat bkRLegbt;

	GLfloat tail;
}still, walk[8], trot[8], gallop[8];

GLvoid initFrames() //creates walking positions
{
    still.neckbt = -80;
	still.necktp = 25;
	still.head = 87;
	still.frtLLegtp = 90;
	still.frtLLegbt = 10;
	still.frtRLegtp = 90;
	still.frtRlegbt = 10;
	still.bkLLegtp = 90;
	still.bkLLegbt = 10;
	still.bkRLegtp = 90;
	still.bkRLegbt = 10;
	still.tail = -50;

	walk[0].neckbt = -81;
	walk[0].necktp = 25;
	walk[0].head = 87;
	walk[0].frtLLegtp = 80;
	walk[0].frtLLegbt = 10;
	walk[0].frtRLegtp = 100;
	walk[0].frtRlegbt = 10;
	walk[0].bkLLegtp = 100;
	walk[0].bkLLegbt = 10;
	walk[0].bkRLegtp = 80;
	walk[0].bkRLegbt = 10;
	walk[0].tail = -40;

	walk[1].neckbt = -82;
	walk[1].necktp = 25;
	walk[1].head = 87;
	walk[1].frtLLegtp = 100;
	walk[1].frtLLegbt = 10;
	walk[1].frtRLegtp = 80;
	walk[1].frtRlegbt = 10;
	walk[1].bkLLegtp = 80;
	walk[1].bkLLegbt = 10;
	walk[1].bkRLegtp = 100;
	walk[1].bkRLegbt = 10;
	walk[1].tail = -45;


}

GLvoid output(float row, float col, std::string out)
{
	int len, i;
	glRasterPos2d(col, row);
	len = (int)out.length();
	for (i = 0; i < len; i++)
	{
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, out[i]);
	}
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
	rotation += 0.1*horseSpeed;
	// and ranges between 0 and 360
	if (rotation > 360.0)
		rotation = 0.0;


	//glDisable(GL_LIGHTING);
	glColor3d(0.1, 0.1, 0.4);

	output(5, -6, "Mignon Kramp / A385 Computer Graphics");
	output(4.5, -6, "- LR move leftright");
	output(4, -6, "- look up or down: A / Z");
	output(3.5, -6, "- look right/left: arrows ->/<-");
	output(3, -6, "- walk forward/backward: arrows UP/Down");
	output(2.5, -6, "XYZ " + std::to_string(horse.angle));

	DrawGround();
	DrawLinesXYZ();
	UpdateHorse();

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
	case 'R' : case 'r':
		horse.angle+=5.0;
		break;
	
	case 'L': case 'l':
		horse.angle -=5.0;
		break;
	
	case '1':
		horseSpeed = 0;
		horse.state = 1;
		break;
	case '2':
		horseSpeed = 0.5;
		horse.state = 2;
		break;
	case '3':
		horseSpeed = 7;
		horse.state = 3;
		break;
	case '4':
		horseSpeed = 15;
		horse.state = 4;
		break;
	default:
	{  
		break;
	}
}

	// keyboard :: normal keys
	switch (normalKEY)
	{   // looking up
	case 'A': case 'a':    
		g_elevationAngle += 2.0; break;

		// looking down
	case 'Z':case 'z':
		g_elevationAngle -= 2.0;  break;

	default:
	{   
		break;
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
//=========================================================//
//=========================================================//
GLvoid UpdateHorse() //cosin and sin take sinputs as radians, and angle converts to radians
{
	if (!horse.initialized)
	{
		initFrames();
	}
	static int frameCounter = 0;

	horse.rad = PI*horse.angle / 180.0;
	horse.x += sin(horse.rad)*horseSpeed / 500.0;
	horse.z += cos(horse.rad)*horseSpeed / 500.0;

	static time_t lasttime = time(nullptr);
	time_t currenttime = time(nullptr);
	if (currenttime - lasttime > 0.001)
	{
		lasttime = currenttime;
		frameCounter++;
		if (frameCounter > 1)
		{
			frameCounter = 0;
		}
	}

	if (horse.state == 1)
	{
		DrawHorse(still);
	}
	else if (horse.state == 2)
	{
		DrawHorse(walk[frameCounter]);
	}
	else if (horse.state == 3)
	{
		DrawHorse(trot[frameCounter]);
	}
	else if (horse.state == 4)
	{
		DrawHorse(gallop[frameCounter]);
	}


	DrawHorse(walk[frameCounter]);
}

//=========================================================//
//=========================================================//
GLvoid DrawHorse(FrameAngles gait)
{
	//Body of Horse
	glPushMatrix();
	glTranslated(horse.x, horse.y+.4, horse.z);
	glRotated(horse.angle, 0, 1, 0);
	glTranslated(0, 1, -.7);
	//DrawLinesXYZ();
	glColor3f(1.0, .57, .67);
	gluCylinder(g_normalObject, 0.35, 0.4, 1.4, 20, 8);
		glPushMatrix(); //make disk move relitive to cylinder
		gluDisk(g_normalObject, 0, 0.35, 20, 8);
		glTranslated(0, 0, 1.4);
		gluDisk(g_normalObject, 0, 0.4, 20, 8);
		glPopMatrix();
		
		//make neck bottom
		glPushMatrix();  
		glTranslatef(0, .4, 1.2);
		//
		glRotated(gait.neckbt, 1, 0, 0);
		gluCylinder(g_normalObject, 0.2, 0.19, 0.5, 20, 8);
			//make neck top
			glPushMatrix();
			glTranslatef(0, 0, 0.5);
			glRotated(gait.necktp, 1, 0, 0);
			gluCylinder(g_normalObject, 0.19, 0.10, 0.5, 20, 8);
				//make Head
				glPushMatrix();
				glTranslatef(0, 0, .5);
				glRotated(gait.head, 1, 0, 0);
				gluCylinder(g_normalObject, 0.2, 0.1, 0.6, 20, 8);
					//make Horn
					glPushMatrix();
					glTranslatef(0, 0.1, 0.2);
					glRotated(-90, 1, 0, 0);
					glColor3f(.5, 1.0, .80);
					gluCylinder(g_normalObject, 0.05, 0, 0.5, 20, 8);
					glPopMatrix();
				//DrawLinesXYZ();
				glPopMatrix();
				glColor3f(1.0, .57, .67);
			glPopMatrix();
		//	DrawLinesXYZ();
		glPopMatrix();

		
		//make Leg Left Front top
		glPushMatrix();
		glTranslatef(0.15, -.3, 1.3);
		glRotated(gait.frtLLegtp, 1, 0, 0);
	    //DrawLinesXYZ();
		gluCylinder(g_normalObject, 0.1, 0.1, 0.6, 20, 8);
			//make Leg Left Front bottom
			glPushMatrix();
			glTranslatef(0, 0, 0.5);
			glRotated(gait.frtLLegbt,10, 0, 0);
			gluCylinder(g_normalObject, 0.1, 0.13, 0.67, 20, 8);
			glPopMatrix();
		glPopMatrix();

		//make Leg Right Front top
		glPushMatrix();
		glTranslatef(-.15, -.3, 1.3);
		glRotated(gait.frtRLegtp, 1, 0, 0);
		gluCylinder(g_normalObject, 0.1, 0.1, 0.6, 20, 8);
			//make Leg Right Front bottom
			glPushMatrix();
			glTranslatef(0, 0, 0.5);
			glRotated(gait.frtRlegbt, 10, 0, 0);
			gluCylinder(g_normalObject, 0.1, 0.13, 0.67, 20, 8);
			glPopMatrix();
		glPopMatrix();
		
		//make Leg Left Back top
		glPushMatrix();
		glTranslatef(0.15, -.3, 0.15);
		glRotated(gait.bkLLegtp, 1, 0, 0);
		gluCylinder(g_normalObject, 0.15, 0.1, 0.6, 20, 8);
			//make left leg back bottom
			glPushMatrix();
			glTranslatef(0, 0, 0.5);
			glRotated(gait.bkLLegbt, -10, 0, 0);
			gluCylinder(g_normalObject, 0.1, 0.13, 0.67, 20, 8);
			glPopMatrix();
		//DrawLinesXYZ();
		glPopMatrix();

		//make right leg back top
		glPushMatrix();
		glTranslatef(-0.15, -.3, 0.15);
		glRotated(gait.bkRLegtp, 1, 0, 0);
		gluCylinder(g_normalObject, 0.15, 0.1, 0.6, 20, 8);
			//make right leg back bottom
			glPushMatrix();
			glTranslatef(0, 0, 0.5);
			glRotated(gait.bkRLegbt, -10, 0, 0);
			gluCylinder(g_normalObject, 0.1, 0.13, 0.67, 20, 8);
			glPopMatrix();
		glPopMatrix();

		//make tail
		glPushMatrix();
		glRotated(gait.tail, 1, 0,0 );
		glTranslated(0, 0.07, -0.5);
		gluCylinder(g_normalObject, 0.18, 0.1, 0.6, 20, 8);
		glPopMatrix();

	
	glPopMatrix(); //End Horse
}
//------------------------------------------Horse Complete
//=========================================================//
//=========================================================//

