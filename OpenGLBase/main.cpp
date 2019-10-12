// This program is from the OpenGL Programming Guide. It shows a robot.
// This program is a modified version of the original source codes https://cs.lmu.edu/~ray/notes/openglexamples/
// This program runs well under the settings you have done for the Assignment1. Please go
// back to settings if you have compile or link errors for this program.

#ifdef __APPLE_CC__
#include <GLUT/glut.h>
#include <GLUT/freeglut.h>
#else
#include <GL/glut.h>
#include <GL/freeglut.h>
#endif
#include <stdio.h>

#include "arcballcamera.h"
#include "bodypart.h";

#define BODY_WIDTH 2
#define BODY_HEIGHT 4
#define BODY_DEPTH 1
// The robot arm is specified by (1) the angle that the upper arm makes
// relative to the x-axis, called shoulderAngle, and (2) the angle that the
// lower arm makes relative to the upper arm, called elbowAngle. These angles
// are adjusted in 5 degree increments by a keyboard callback.

/*Create a bunch of robot bodyparts
Each bodypart has a unique position and rotation,
that can be changed with a couple different functions I added.
*/
static robot::bodypart
	left_forearm,
	right_forearm,
	left_upperarm,
	right_upperarm,
	left_upperleg,
	right_upperleg,
	left_lowerleg,
	right_lowerleg;

static bool wireframe = true;
static bool showAxis = true;


// Handles the keyboard event: the left and right arrows bend the elbow, the
// up and down keys bend the shoulder.
void special(int key, int, int) {
	switch (key) {
	case GLUT_KEY_LEFT:  left_forearm.incrementAngle();  right_forearm.decrementAngle();  break;
	case GLUT_KEY_RIGHT: left_forearm.decrementAngle();  right_forearm.incrementAngle();  break;
	case GLUT_KEY_UP:    left_upperarm.incrementAngle(); right_upperarm.decrementAngle(); break;
	case GLUT_KEY_DOWN:  left_upperarm.decrementAngle(); right_upperarm.incrementAngle(); break;
	case GLUT_KEY_END:   exit(0);
	default: return;
	}
	glutPostRedisplay();
}

// wireBox(w, h, d) makes a wireframe box with width w, height h and
// depth d centered at the origin. It uses the GLUT wire cube function.
// The calls to glPushMatrix and glPopMatrix are essential here; they enable
// this function to be called from just about anywhere and guarantee that
// the glScalef call does not pollute code that follows a call to myWireBox.
void solidBox(GLdouble width, GLdouble height, GLdouble depth) {
	glPushMatrix();
	glColor3f(1.0, 1.0, 1.0);
	glScalef(width, height, depth);
	glutSolidCube(1.0);
	glPopMatrix();
}

void drawAxes()
{
	// Draw a red x-axis, a green y-axis, and a blue z-axis.
	glBegin(GL_LINES);
	glColor3f(1, 0, 0); glVertex3f(0, 0, 0); glVertex3f(5, 0, 0);
	glColor3f(0, 1, 0); glVertex3f(0, 0, 0); glVertex3f(0, 5, 0);
	glColor3f(0, 0, 1); glVertex3f(0, 0, 0); glVertex3f(0, 0, 5);
	glEnd();
}
void drawScene()
{

	glPushMatrix();
	
	gluLookAt(x, y, z, //camera is located at (x,y,z)
		0, 0, 0, //camera is looking at (0,0,0)
		0.0f, 1.0f, 0.0f); //up vector is (0,1,0) (positive Y)
	
	if (showAxis) drawAxes(); // draw axes


	// Draw the upper body at the orgin
	solidBox(BODY_WIDTH, BODY_HEIGHT, BODY_DEPTH);
	// Draw the upper arm, rotated shoulder degrees about the z-axis. Note that
	// the thing about glutWireBox is that normally its origin is in the middle
	// of the box, but we want the "origin" of our box to be at the left end of
	// the box, so it needs to first be shifted 1 unit in the x direction, then
	// rotated.
	

	/*Draws Head*/
	glPushMatrix();

		glTranslatef(0.0, 3.0, 0.0);
		glutSolidSphere(1.0, 50, 50);

	glPopMatrix();
	/*
		Draws Right Arms
		Both the forearm, and upperarm share the same rotation matrix,
		because the rotation of the upperarm affects the final rotation of
		the forearm. This is the same for the rest of the components.
	*/
	glPushMatrix();

		right_upperarm.setPosition(1.0, 1.5, 0.0);
		right_upperarm.draw();

		right_forearm.setPosition(1.0, 0.0, 0.0);
		right_forearm.draw();

	glPopMatrix();

	/*Same thing with this, although instead of applying another pushmatrix to mirror the
	object, I made it adjust the angles, and center of rotation, just for my own practice.
	Really, it would probably be easier to rotate the whole arm.
	*/
	glPushMatrix();

		left_upperarm.setPosition(-1.0, 1.5, 0.0);
		left_upperarm.setRotationPoint(-1.0,0.0,0.0);
		left_upperarm.draw();

		left_forearm.setPosition(-1.0, 0.0, 0.0);
		left_forearm.draw();

	glPopMatrix();

	/*Since i didn't feel like figuring out the correct paramters to initalize the robot legs,
	i used the same parameters as a robot arm, and applied a rotation, (using push and pop matricies),
	to rotate the whole arm into the position of the leg. 
	*/
	glPushMatrix();
		glTranslatef(.8, -2.0, 0.0);
		glRotatef(90, -1.0, 0.0, 0.0);
		glPushMatrix();
			glRotatef(90, 0.0, 1.0, 0.0);
			glPushMatrix();
				right_upperleg.setPosition(0.0, 0.0, 0.0);
				right_upperleg.draw();

				right_lowerleg.setPosition(1.0, 0.0, 0.0);
				right_lowerleg.draw();
			glPopMatrix();
		glPopMatrix();
	glPopMatrix();


	/*Same thing with tis leg. Only thing is it is translated slightly to the left, instead of slightly the right.
	*/
	glPushMatrix();
		glTranslatef(-.8, -2.0, 0.0);
		glRotatef(90, -1.0, 0.0, 0.0);
		glPushMatrix();
			glRotatef(90, 0.0, 1.0, 0.0);
			glPushMatrix();
				left_upperleg.setPosition(0.0, 0.0, 0.0);
				left_upperleg.draw();

				left_lowerleg.setPosition(1.0, 0.0, 0.0);
				left_lowerleg.draw();
			glPopMatrix();
		glPopMatrix();
	glPopMatrix();

	glPopMatrix();
}
// Displays the arm in its current position and orientation. The whole
// function is bracketed by glPushMatrix and glPopMatrix calls because every
// time we call it we are in an "environment" in which a gluLookAt is in
// effect. (Note that in particular, replacing glPushMatrix with
// glLoadIdentity makes you lose the camera setting from gluLookAt).
void display() {
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);


	/*Render models as wireframe if enabled*/
	if (wireframe) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else           glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	drawScene(); // draw a robot
	glFlush();
}
// Handles the reshape event by setting the viewport so that it takes up the
// whole visible region, then sets the projection matrix to something reason-
// able that maintains proper aspect ratio.

void reshape(GLint w, GLint h) {
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(65.0, GLfloat(w) / GLfloat(h), 1.0, 200.0);
}
void procKeys(unsigned char key, int x, int y)
{
	switch (key) {
		case '1':  wireframe = true; break;
		case '2':  wireframe = false; break;
		case '3':  showAxis = !showAxis; break;
		case 'w':  left_upperleg.incrementAngle(); right_upperleg.incrementAngle(); break;
		case 's':  left_upperleg.decrementAngle(); right_upperleg.decrementAngle(); break;
		case 'a':  left_lowerleg.incrementAngle(); right_lowerleg.incrementAngle(); break;
		case 'd':  left_lowerleg.decrementAngle(); right_lowerleg.decrementAngle(); break;
		case 27: exit(0); break;
	}
	glutPostRedisplay();
}
// Perfroms application specific initialization: turn off smooth shading,
// sets the viewing transformation once and for all. In this application we
// won't be moving the camera at all, so it makes sense to do this.
void init() {
	glShadeModel(GL_FLAT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	left_forearm.setAngle(180);
}
// Initializes GLUT, the display mode, and main window; registers callbacks;
// does application initialization; enters the main event loop.
int main(int argc, char** argv) {
	printf("\n\
-----------------------------------------------------------------------\n\
 OpenGL Sample Program for a robot:\n\
 - 'LEFT CLICK AND DRAG': Rotate \n\
 - 'RIGHT CLICK AND DRAG': ZOOM \n\
 - 'MIDDLE MOUSE': ZOOM \n\
 - '1': show wireframe \n\
 - '2': show solid \n\
 - '3': toggle axis \n\
 - 'w': increase the upper leg angle \n\
 - 's': decrease the upper leg angle \n\
 - 'a': increase the lower leg angle \n\
 - 'd': increase the lower leg angle \n\
 - LEFT ARROW: increment the elbowAngle \n\
 - RIGHT ARROW: decrement the elbowAngle \n\
 - UP ARROW: increment shoulderAngle \n\
 - DOWN ARROW: decrement shoulderAngle \n\
 - ESC to quit\n\
-----------------------------------------------------------------------\n");
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowPosition(80, 80);
	glutInitWindowSize(800, 600);
	glutCreateWindow("Max Kotlan - Robot Arm");

	cameraRadius = 7.0f;
	cameraTheta = 2.80;
	cameraPhi = 2.0;
	recomputeOrientation();

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(procKeys);
	glutSpecialFunc(special);
	glutMouseFunc(mouseCallback);
	glutMotionFunc(mouseMotion);
	glutMouseWheelFunc(mouseWheel);

	init();
	glutMainLoop();
}