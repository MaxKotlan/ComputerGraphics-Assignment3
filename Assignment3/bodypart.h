#ifndef ROBOT_BODYPART_H
#define ROBOT_BODYPART_H

#ifdef __APPLE_CC__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <stdio.h>

/*Import solidBox function from main (compiled in main. linked later). I left it in main, because it was there in the original file,
and I don't want to change too many things. A better place for it would probably be here. I also renamed it from wireBox to solidbox, because
I am drawing everything solid, then using a different way to render it as wireframe.
*/
void solidBox(GLdouble width, GLdouble height, GLdouble depth);

/*Created a class called robot body part,
that essentially renders a square at a solid box at a specific position and rotation.

This class also allows you to change the center of rotation on the object, and keeps track of the current
angle.
*/
namespace robot {
	class bodypart {
	private:
		const int _angleIncrement = 5;
		int _angle = 0;

		GLfloat _postion[3]		  = { 0.0, 0.0, 0.0 };
		GLfloat _color[3]		  = { 1.0, 1.0, 1.0 };
		GLfloat _size[3]	      = { 2.0, 0.4, 1.0 };
		GLfloat _rotationPoint[3] = { 1.0, 0.0, 0.0 };

	public:
		void draw();
		void setAngle(int newAngle);
		void incrementAngle();
		void decrementAngle();
		void setPosition(GLfloat x, GLfloat y, GLfloat z);
		void setRotationPoint(GLfloat x, GLfloat y, GLfloat z);
	};
}

void robot::bodypart::draw()
{
	glTranslatef(_postion[0], _postion[1], _postion[2]);
	glRotatef((GLfloat)_angle, 0.0, 0.0, 1.0);
	glTranslatef(_rotationPoint[0], _rotationPoint[1], _rotationPoint[2]);
	solidBox(_size[0], _size[1], _size[2]);
}

inline void robot::bodypart::setAngle(int newAngle)
{ _angle = newAngle; }

inline void robot::bodypart::incrementAngle()
{ (_angle += _angleIncrement) %= 360; }

inline void robot::bodypart::decrementAngle()
{ (_angle -= _angleIncrement) %= 360; }

inline void robot::bodypart::setPosition(GLfloat x, GLfloat y, GLfloat z)
{ _postion[0] = x; _postion[1] = y; _postion[2] = z; }

inline void robot::bodypart::setRotationPoint(GLfloat x, GLfloat y, GLfloat z)
{ _rotationPoint[0] = x; _rotationPoint[1] = y; _rotationPoint[2] = z; }

#endif