#include <cmath>
#include <iostream>

#include "galaxy.h"
#include "star.h"

const double HALF_PI = 1.57079632679;

Galaxy::Galaxy(int numStars, float avgStarMass, GLint galaxyRadius)
{
  /*Candidate coordinates for a new star*/
  GLint Xc, Yc;
  /*This will be a random number at most massDistro away from avgStarMass*/
  float mass;
  /*Each star will get different component velocities assigned.*/
  float xVel, yVel;
  /*This is the actual distance from center that our coordinate candidates have*/
  GLint distFromCenterSquared;
  int i = 0;

  while(i < numStars)
    {
      //generate possible coordinates for the new star.
      Xc = (rand() % (2 *  galaxyRadius) - galaxyRadius);
      Xc += (rand() % (2 *  galaxyRadius) - galaxyRadius);
      Xc /= 2;
      Yc = rand() % (2 *  galaxyRadius) - galaxyRadius;
      Yc += rand() % (2 *  galaxyRadius) - galaxyRadius;
      Yc /= 2;
      mass = avgStarMass;
      distFromCenterSquared = Xc * Xc + Yc * Yc;

      //are these coordinates within bounds?
      if (distFromCenterSquared <= galaxyRadius * galaxyRadius)
	{
	  star_list.push_back(Star(Xc, Yc, mass));
	  i++;
	}
    }
  setSpinning();
}

void Galaxy::halt()
{
  std::list<Star>::iterator starIter = star_list.begin();
  while(starIter != star_list.end())
    {
      starIter->sXVel = 0;
      starIter->sYVel = 0;
    }
}

void Galaxy::setSpinning() 
{
  std::list<Star>::iterator starIter, starIter2, tempIter;
  float grav, distanceFromCenterSquared;
  /*Now, the velocities need to be generated.
    They all start in an orbital pattern, so
    the centrifugal force needs to balance out with the force of gravity.
    Fcentrifugal = (m * v^2 / r) * (unit vector pointing outward)
  */
  //initialize all forces to 0
  for(starIter = star_list.begin(); starIter != star_list.end(); starIter++)
    {
      starIter->clearForces();
    }

  //determine the force of gravity.  We'll assume the gravity vector points to the exact center.
 for(starIter = star_list.begin(); starIter != star_list.end(); starIter++)
    {
      starIter2 = starIter;
      starIter2++;
      while(starIter2 != star_list.end())
	{
       	  updateGravity(*starIter, *starIter2);
	  starIter2++;
	}
    }
 //Now set every star's starting velocities
  for(starIter = star_list.begin(); starIter != star_list.end(); starIter++)
    {
      float grav = sqrt(starIter->sXGrav * starIter->sXGrav + starIter->sYGrav * starIter->sYGrav);
      float radius = sqrt(starIter->sX * starIter->sX + starIter->sY * starIter->sY);
      float speed = sqrt(radius * grav / starIter->sMass);
      float theta = atan2(starIter->sYGrav , starIter->sXGrav);
      starIter->sXVel = speed * cos(theta + HALF_PI);
      starIter->sYVel = speed * sin(theta + HALF_PI);
    }

}

bool Galaxy::out_of_bounds(Star &star)
{
  bool ret_val = false;

  if ((abs((int) star.sX) > 5000) || (abs((int) star.sY) > 5000)) {
    ret_val = true;
  }
  return ret_val;
}

void Galaxy::update(float timeDelta)
{
  std::list<Star>::iterator starIter, starIter2, tempIter;

  starIter = star_list.begin();
  while (starIter != star_list.end() ) {
    starIter->updateDisplacement(timeDelta);
    if(out_of_bounds(*starIter) ) {
      tempIter = starIter;
      starIter++;
      star_list.erase(tempIter);      
    }
    else {
      starIter->clearForces();
      starIter++;
    }
  }


  /*    update gravity forces and resultant velocity*/
  for(starIter = star_list.begin(); starIter != star_list.end(); starIter++)
    {
      /*We do not want to calculate the gravitational forces a star has on itself*/
      starIter2 = starIter;
      starIter2++;
      while(starIter2 != star_list.end())
	{
	  /*If the two stars have collided, add the second to the first,
	    delete the second, and continue.  This means some innacuracy
	    when calculating the gravity forces when two stars collide.
	    However, this means a large performance improvement */
	  if (getDistance(*starIter, *starIter2) <= 0)
	    {
	      *starIter += *starIter2;
	      tempIter = starIter2;
	      starIter2++;
	      star_list.erase(tempIter);
	    }
	  else
	    {
	      updateGravity(*starIter, *starIter2);
	      starIter2++;
	    }
	}
    }
}

void Galaxy::draw()
{
  for(std::list<Star>::iterator starIter = star_list.begin();
      starIter != star_list.end(); starIter++)
    starIter->draw();
}
