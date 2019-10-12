#ifndef ARCBALLCAMERA_H
#define ARCBALLCAMERA_H

#ifdef __APPLE_CC__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <stdio.h>
#include <math.h>
#define PI 3.141592
/*
This file was mainly copied from the lecture notes,
CG_L12_13_std_updated. I put it in a seperate file,
because main was getting unorganized
*/


int mouseX = 0, mouseY = 0; //last known X and Y of the mouse
float cameraTheta, cameraPhi, cameraRadius; //camera position in spherical coordinates
float x, y, z; //camera position in cartesian coordinates
GLint leftMouseButton, rightMouseButton; //status of the mouse buttons
void recomputeOrientation()
{
	x = cameraRadius * sinf(cameraTheta) * sinf(cameraPhi);
	z = cameraRadius * cosf(cameraTheta) * sinf(cameraPhi);
	y = cameraRadius * cosf(cameraPhi);
	
	glutPostRedisplay();
}

void mouseCallback(int button, int state, int thisX, int thisY)
{
	//update the left and right mouse button states, if applicable
	if (button == GLUT_LEFT_BUTTON)
		leftMouseButton = state;
	else if (button == GLUT_RIGHT_BUTTON)
		rightMouseButton = state;
	//and update the last seen X and Y coordinates of the mouse
	mouseX = thisX;
	mouseY = thisY;
}


void mouseMotion(int x, int y)
{
	if (leftMouseButton == GLUT_DOWN)
	{
		cameraTheta += (mouseX - x) * 0.005;
		cameraPhi += (mouseY - y) * 0.005;
		// make sure that phi stays within the range (0, PI)
		if (cameraPhi <= 0)
			cameraPhi = 0 + 0.001;
		if (cameraPhi >= PI)
			cameraPhi = PI - 0.001;
		recomputeOrientation(); //update camera (x,y,z)
	} // camera zoom in/out
	else if (rightMouseButton == GLUT_DOWN) {
		double totalChangeSq = (x - mouseX) + (y - mouseY);
		cameraRadius += totalChangeSq * 0.01;
		//limit the camera radius to some reasonable values so the user can't get lost
		if (cameraRadius < 2.0)
			cameraRadius = 2.0;
		if (cameraRadius > 300.0)
			cameraRadius = 300.0;
		recomputeOrientation(); //update camera (x,y,z) based on (radius,theta,phi)
	}
	mouseX = x;
	mouseY = y;
}
#endif