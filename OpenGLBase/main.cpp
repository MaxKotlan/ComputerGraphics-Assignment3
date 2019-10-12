// This program is from the OpenGL Programming Guide. It shows a robot.
// This program is a modified version of the original source codes https://cs.lmu.edu/~ray/notes/openglexamples/
// This program runs well under the settings you have done for the Assignment1. Please go
// back to settings if you have compile or link errors for this program.

#ifdef __APPLE_CC__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <stdio.h>

#include "bodypart.h";

#define BODY_WIDTH 2
#define BODY_HEIGHT 4
#define BODY_DEPTH 1
// The robot arm is specified by (1) the angle that the upper arm makes
// relative to the x-axis, called shoulderAngle, and (2) the angle that the
// lower arm makes relative to the upper arm, called elbowAngle. These angles
// are adjusted in 5 degree increments by a keyboard callback.

static robot::bodypart
	left_forearm,
	right_forearm,
	left_upperarm,
	right_upperarm,
	left_upperleg,
	right_upperleg,
	left_lowerleg,
	right_lowerleg;


static int shoulderAngle = 0, elbowAngle = 0;
bool baxis = true;
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
void wireBox(GLdouble width, GLdouble height, GLdouble depth) {
	glPushMatrix();
	glColor3f(1.0, 1.0, 1.0);
	glScalef(width, height, depth);
	glutWireCube(1.0);
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
	// Draw the upper body at the orgin
	wireBox(BODY_WIDTH, BODY_HEIGHT, BODY_DEPTH);
	// Draw the upper arm, rotated shoulder degrees about the z-axis. Note that
	// the thing about glutWireBox is that normally its origin is in the middle
	// of the box, but we want the "origin" of our box to be at the left end of
	// the box, so it needs to first be shifted 1 unit in the x direction, then
	// rotated.
	
	//glTranslatef(1.0, 1.5, 0.0); // (4) move to the right end of the upper body (attachment)
	//glRotatef((GLfloat)shoulderAngle, 0.0, 0.0, 1.0); //(3) then rotate shoulder
	//glTranslatef(1.0, 0.0, 0.0); // (2) shift to the right on the x axis to have the left end at the origin
	//wireBox(2.0, 0.4, 1.0); // (1) draw the upper arm box
	glPushMatrix();

		glTranslatef(0.0, 3.0, 0.0);
		glutWireSphere(1.0, 50, 50);

	glPopMatrix();
	glPushMatrix();

		right_upperarm.setPosition(1.0, 1.5, 0.0);
		right_upperarm.draw();

		right_forearm.setPosition(1.0, 0.0, 0.0);
		right_forearm.draw();

	glPopMatrix();
	glPushMatrix();

		left_upperarm.setPosition(-1.0, 1.5, 0.0);
		left_upperarm.setRotationPoint(-1.0,0.0,0.0);
		left_upperarm.draw();

		left_forearm.setPosition(-1.0, 0.0, 0.0);
		left_forearm.draw();

	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0, -2, 0.0);
	glRotatef(90, 1.0, 0.0, 0.0);
	glPushMatrix();
		glRotatef(90, 0.0, -1.0, 0.0);
		glPushMatrix();
			right_upperleg.setPosition(0.8 * (BODY_WIDTH / 2.0), -(BODY_HEIGHT / 2.0), 0.0);
			right_upperleg.draw();

			right_lowerleg.setPosition(1.0, 0.0, 0.0);
			right_lowerleg.draw();
		glPopMatrix();
	glPopMatrix();
	glPopMatrix();

	// Now we are ready to draw the lower arm. Since the lower arm is attached
	// to the upper arm we put the code here so that all rotations we do are
	// relative to the rotation that we already made above to orient the upper
	// arm. So, we want to rotate elbow degrees about the z-axis. But, like
	// before, the anchor point for the rotation is at the end of the box, so
	// we translate <1,0,0> before rotating. But after rotating we have to
	// position the lower arm at the end of the upper arm, so we have to
	// translate it <1,0,0> again.
	//glTranslatef(1.0, 0.0, 0.0); // (4) move to the right end of the upper arm
	//glRotatef((GLfloat)elbowAngle, 0.0, 0.0, 1.0); // (3) rotate
	//glTranslatef(1.0, 0.0, 0.0); // (2) shift to the right on the x axis to have the left end at the origin
	//wireBox(2.0, 0.4, 1.0); // (1) draw the lower arm
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
	drawScene(); // draw a robot
	drawAxes(); // draw axes
	glFlush();
}
// Handles the reshape event by setting the viewport so that it takes up the
// whole visible region, then sets the projection matrix to something reason-
// able that maintains proper aspect ratio.

void reshape(GLint w, GLint h) {
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(65.0, GLfloat(w) / GLfloat(h), 1.0, 20.0);
}
void procKeys(unsigned char key, int x, int y)
{
	switch (key) {
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
	gluLookAt(1, 2, 8, 0, 0, 0, 0, 1, 0);

	left_forearm.setAngle(180);
}
// Initializes GLUT, the display mode, and main window; registers callbacks;
// does application initialization; enters the main event loop.
int main(int argc, char** argv) {
	printf("\n\
-----------------------------------------------------------------------\n\
 OpenGL Sample Program for a robot:\n\
 - 'a': toggle on off to draw axes \n\
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
	glutCreateWindow("Robot Arm");
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(procKeys);
	glutSpecialFunc(special);
	init();
	glutMainLoop();
}