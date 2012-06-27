#include <iostream>
#include <cmath>
#include <cstdlib>

#include <GL/gl.h>
#include <GL/glut.h>

#include "star.h"

using namespace std;

GLuint Star::star_CallList = 0;

const GLfloat PI = 3.14159265359;
const GLfloat MASS_TO_RAD_MULTIPLIER = pow(.75 / PI, .33333333333333333);
float getDistance(const Star &left, const Star &right)
{
  float deltaX = left.sX - right.sX;
  float deltaY = left.sY - right.sY;
  return (sqrt((deltaX * deltaX) + (deltaY * deltaY)) - left.getRadius() - right.getRadius());
}

void updateGravity(Star &left, Star &right)
{
  float deltaX = right.sX - left.sX;
  float deltaY = right.sY - left.sY;
  float gravity_force = (left.sMass * right.sMass) / ((deltaX * deltaX) + (deltaY * deltaY));
  float theta = atan2f(deltaY, deltaX);
  float xGrav = gravity_force * cos(theta);
  float yGrav = gravity_force * sin(theta);
  left.sXGrav += xGrav;
  left.sYGrav += yGrav;
  right.sXGrav -= xGrav;
  right.sYGrav -= yGrav;
}

void Star::updateDisplacement(float timeDelta)
{
  float timeDeltaOverMass;

  //time elapsed in seconds / mass
  timeDeltaOverMass = timeDelta / sMass;

  /*V = Vo + at */
  sXVel += sXGrav * timeDeltaOverMass;
  sYVel += sYGrav * timeDeltaOverMass;

  sX += sXVel * timeDelta;
  sY += sYVel * timeDelta;
}

Star::Star(GLfloat x, GLfloat y, float mass,
	   float xVel, float yVel)
{
  GLfloat white[4] = {1.0, 1.0, 1.0, 1.0};
  sX = x;
  sY = y;
  sXVel = xVel;
  sYVel = yVel;
  sMass = mass;

  sMassChanged = true;
  sXGrav = sYGrav = 0;

  if (!star_CallList)
    {
      star_CallList = glGenLists(1);
      glNewList(star_CallList, GL_COMPILE);
      defineStar();
      glEndList();
    }
};

void Star::defineStar()
{
  glutSolidSphere(1.0,20,20);
}

void Star::draw()
{
  GLfloat radius = (GLfloat) getRadius();
  glPushMatrix();
  glTranslatef(sX, sY, 0.0);
  glScalef(radius, radius, radius);
  glCallList(star_CallList);
  glPopMatrix();
}

float Star::getRadius() const
{
  if(sMassChanged)
    {
      sRadius = 5.0 * pow( (double) sMass , .3333333333333333333);
      sMassChanged = false;
    }
  return sRadius;
}

Star &Star::operator+=(const Star &right)
{
  float totalMass = sMass + right.sMass;
  

  /*This is done using the law of conservation of momentum*/
  sXVel = (sMass * sXVel + right.sMass * right.sXVel) / totalMass;
  sYVel = (sMass * sYVel + right.sMass * right.sYVel) / totalMass;

  sMass = totalMass;
  sXGrav += right.sXGrav;
  sYGrav += right.sYGrav;

  sMassChanged = true;

  float deltaFactor = right.getRadius() / getRadius();
  sX += (right.sX - sX) * deltaFactor;
  sY += (right.sY - sY) * deltaFactor;
  
  return (*this);
}

