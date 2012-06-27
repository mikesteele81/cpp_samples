#ifndef GALAXY_H
#define GALAXY_H

#include <list>

#include <GL/gl.h>
#include "SDL/SDL.h"

#include "star.h"

class Star;

class Galaxy
{
 public:
  Galaxy(int numStars = 500, float avgStarMass = 100.0, GLint galaxyRadius = 10000);
  void draw();//Draw the galaxy in its present state.
  void update(float timeDelta);//Update everything in the galaxy.
  void spin() { setSpinning();};
  void halt();
 private:
  void setSpinning();//Set velocities of all stars so they are orbiting each other
  bool out_of_bounds(Star &star);
  std::list<Star> star_list;
};

#endif
