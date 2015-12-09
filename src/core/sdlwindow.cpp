#include "../common/platform.h"
#include "sdlwindow.h"
#include "engine.h"
#include "world.h"
#include <iostream>
#include <stdio.h>

SDLWindow::SDLWindow( const char* title ) : Window( title ), hasContext( false ), hasWindow( false )
{}

SDLWindow::~SDLWindow() {
	cleanup();
}

void
SDLWindow::create()
{
	if (!createWindow())
		return;
	updateViewportDimensions( );
}

void 
SDLWindow::update( float deltatime ) {
	if( !engine )
		return;
	
	SDL_GL_MakeCurrent(window, glcontext);
	
	for( int i =0; i < viewportCount(); i++ )
	{
		if( viewports[i].world )
			viewports[i].world->update( deltatime );
		glViewport( viewports[i].x, viewports[i].y, viewports[i].width, viewports[i].height );
		engine->draw( &viewports[i] );
		
	}
	
	SDL_GL_SwapWindow(window);	
}

bool
SDLWindow::createWindow()
{
	int i;

	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
 
	//create a window
	if((window = SDL_CreateWindow( title,
                          SDL_WINDOWPOS_UNDEFINED,
                          SDL_WINDOWPOS_UNDEFINED,
                          width, height,
                          SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN)) == NULL)
	{
        return false;
    }
    hasWindow =true;
    
    glcontext = SDL_GL_CreateContext( window );
	
	if( !glcontext ) {
		std::cerr << "SDLApplication: Unable initialize GL context" << std::endl;
		return false;
	}
	hasContext =true;
	
	glewExperimental = GL_TRUE; 
	if (glewInit() != GLEW_OK)
	{
		return false;
	}
	
	if(GLEW_VERSION_1_1)
	{
	  printf("----------------------------------------------------------------\n");
	  printf("Graphics Successfully Initialized\n");
	  printf("OpenGL Info\n");
	  printf("    Version: %s\n", glGetString(GL_VERSION));
	  printf("     Vendor: %s\n", glGetString(GL_VENDOR));
	  printf("   Renderer: %s\n", glGetString(GL_RENDERER));
	  printf("    Shading: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
	  printf("----------------------------------------------------------------\n");
	}
	else
	{
	  printf("Unable to get any OpenGL version from GLEW!");
	}
    SDL_GL_MakeCurrent(window, glcontext);
	
	return true;
}

void
SDLWindow::cleanup()
{
	if( hasContext )
		SDL_GL_DeleteContext(glcontext);
}

