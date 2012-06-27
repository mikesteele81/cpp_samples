#ifndef STAR_H
#define STAR_H

#include <GL/gl.h>
#include <cmath>

#include "galaxy.h"

class Star
{
  friend void updateGravity(Star &left, Star &right);
  friend float getDistance(const Star &left, const Star &right);
  friend class Galaxy;
 public:
  Star(GLfloat x, GLfloat y, float mass = 1000.0,
       float xVel = 0.0, float yVel = 0.0);
  void draw();
  Star &operator+=(const Star &right);
  float getRadius() const;
  void setVelocity(float xVel = 0, float yVel = 0)
    {sXVel = xVel; sYVel = yVel;};
  void updateDisplacement(float timeDelta);
  void clearForces() { sXGrav = 0; sYGrav = 0;};
  const float getXGrav() const {return sXGrav;};
  const float getYGrav() const {return sYGrav;};
 private:
  void defineStar();
  GLfloat sX, sY;
  //component velocities
  float sXVel, sYVel;
  //mass
  float sMass;
  mutable float sRadius;
  //force of gravity
  float sXGrav, sYGrav;
  static GLuint star_CallList;
  /*This is true if the mass has changed since the last getRadius()
    call */
  mutable bool sMassChanged;
};

#endif
