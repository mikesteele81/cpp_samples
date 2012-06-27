/*This is a skeleton program meant to quickly start an
  SDL/OpenGL program
*/

#include <iostream>

#include "SDL/SDL.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include "galaxy.h"

const int SCREEN_WIDTH = 600;
const int SCREEN_HEIGHT = 600;
const int SCREEN_BPP = 16;
const float TIME_COMPRESSION = 10.0;

/* function to release/destroy our resources and restoring the old desktop */
void Quit()
{
  /* clean up the window */
  SDL_Quit( );
}

/* function to reset our viewport after a window resize */
int resizeWindow( int width, int height )
{
  /* Height / width ration */
  GLfloat ratio;
  
  /* Protect against a divide by zero */
  if ( height == 0 )
      height = 1;
    
  ratio = ( GLfloat )width / ( GLfloat )height;
  
  /* change to the projection matrix and set our viewing volume. */
  glMatrixMode( GL_PROJECTION );
  glLoadIdentity( );
  /* Set our perspective */
  glOrtho(-5000.0, 5000.0, -5000.0, 5000.0, -100.0, 100.0);
  //gluPerspective( 45.0f, ratio, 0.1f, 10000.0f );
  
  /* Make sure we're chaning the model view and not the projection */
  glMatrixMode( GL_MODELVIEW );
  /* Reset The View */
  glLoadIdentity( );
  glViewport( 0, 0, ( GLsizei )width, ( GLsizei )height );
  
  return( true );
}

/* function to handle key press events */
void handleKeyPress( SDL_Surface *surface, SDL_keysym *keysym, Galaxy *galaxy)
{
  switch ( keysym->sym )
    {
    case SDLK_ESCAPE:
      /* ESC key was pressed */
      Quit();
      break;
    case SDLK_F1:
      /* F1 key was pressed
       * this toggles fullscreen mode
       */
      SDL_WM_ToggleFullScreen( surface );
      break;
    case SDLK_h:
      galaxy->halt();
      break;
    default:
      break;
    }
}/* general OpenGL initialization function */

int initGL( GLvoid )
{
  GLfloat mat_specular[] = {1.0, 1.0, 1.0, 1.0 };
  GLfloat mat_shininess[] = {50.0};
  GLfloat white_light[] = {1.0, 1.0, 1.0, 1.0};
  GLfloat lmodel_ambient[] = {.5, .5, .5, 1.0};
  GLfloat light_position[] = {0.0, 0.0, 2000.0, 0.0};
  
  /* Set the background black */
  glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
  
  /* Depth buffer setup */
  glClearDepth( 1.0f );

  glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
  glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white_light);
  glLightfv(GL_LIGHT0, GL_POSITION, light_position);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, white_light);
  glLightfv(GL_LIGHT0, GL_SPECULAR, white_light);
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
  
  /* Enables Depth Testing */
  glEnable( GL_DEPTH_TEST );
  //glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_NORMALIZE);
  glEnable(GL_CULL_FACE);
  
  /* The Type Of Depth Test To Do */
  glDepthFunc( GL_LEQUAL );
  
  /* Really Nice Perspective Calculations */
  glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );

  glColor3f(1.0, 1.0, 1.0);
  
  return( true);
}

/* Here goes our drawing code */
int drawGLScene(Galaxy &galaxy)
{
  /* Clear The Screen And The Depth Buffer */
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
  
  galaxy.draw();
  
  /* Draw it to the screen */
  SDL_GL_SwapBuffers( );
  
  return true;
}



int main(int argc, char *argv[])
{
  
  SDL_Surface *surface;
  /* Flags to pass to SDL_SetVideoMode */
  int videoFlags;
  /* main loop variable */
  bool done = false;
  /* used to collect events */
  SDL_Event event;
  /* this holds some info about our display */
  const SDL_VideoInfo *videoInfo;

  Uint32 timeLastUpdate, timeDelta;

  /* initialize SDL */
  if ( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_TIMER ) < 0 )
    {
      fprintf( stderr, "Video initialization failed: %s\n",
	       SDL_GetError( ) );
      Quit();
    }

  /* Fetch the video info */
  videoInfo = SDL_GetVideoInfo( );
  
  if ( !videoInfo )
    {
      fprintf( stderr, "Video query failed: %s\n",
	       SDL_GetError( ) );
      Quit();
    }
  
  /* the flags to pass to SDL_SetVideoMode */
  videoFlags  = SDL_OPENGL;          /* Enable OpenGL in SDL */
  videoFlags |= SDL_GL_DOUBLEBUFFER; /* Enable double buffering */
  videoFlags |= SDL_HWPALETTE;       /* Store the palette in hardware */
  videoFlags |= SDL_RESIZABLE;       /* Enable window resizing */
  
  /* This checks to see if surfaces can be stored in memory */
  if ( videoInfo->hw_available )
    videoFlags |= SDL_HWSURFACE;
  else
    videoFlags |= SDL_SWSURFACE;
  
  /* This checks if hardware blits can be done */
  if ( videoInfo->blit_hw )
    videoFlags |= SDL_HWACCEL;
  
  /* Sets up OpenGL double buffering */
  SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
  
  /* get a SDL surface */
  surface = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP,
			      videoFlags );
  
  /* Verify there is a surface */
  if ( !surface )
    {
      fprintf( stderr,  "Video mode set failed: %s\n", SDL_GetError( ) );
      Quit();
    }
  
  /* initialize OpenGL */
  glutInit(&argc, argv);
  initGL( );
  timeLastUpdate = SDL_GetTicks();
  
  /* resize the initial window */
  resizeWindow( SCREEN_WIDTH, SCREEN_HEIGHT );

  Galaxy galaxy;

  
  /* wait for events */
  while ( !done )
    {
      /* handle the events in the queue */
      
            while ( SDL_PollEvent( &event ) )
	{
	  switch( event.type )
	    {
	    case SDL_VIDEORESIZE:
	      // handle resize event
	      surface = SDL_SetVideoMode( event.resize.w,
					  event.resize.h,
					  16, videoFlags );
	      if ( !surface )
		{
		  fprintf( stderr, "Could not get a surface after resize: %s\n", SDL_GetError( ) );
		  Quit();
		}
	      resizeWindow( event.resize.w, event.resize.h );
	      break;
	    case SDL_KEYDOWN:
	      // handle key presses 
	      handleKeyPress(surface, &event.key.keysym, &galaxy);
	      break;
	    case SDL_QUIT:
	      // handle quit requests
	      done = true;
	      break;
	    default:
	      break;
	    }
	}
      

      /* draw the scene */
      if((timeDelta = SDL_GetTicks() - timeLastUpdate) > 50)
	{
	  galaxy.update((float) timeDelta / TIME_COMPRESSION);
	  drawGLScene(galaxy);
	  timeLastUpdate += timeDelta;
	}
    }
  
  /* clean ourselves up and exit */
  Quit();
  
  /* Should never get here */
  return( 0 );
}
