/*
 * This file is part of Folia, an experimental mini-engine using OpenGL 3
 * Created by Micky Faas. Freely usable and modifiable for academic purposes.
 */

#include "sdlapplication.h"
#include "../common/platform.h"
#include "../utils/shaderprogram.h"
#include "window.h"
#include <iostream>
#include <stdio.h>

Application*
Application::instance =0;

SDLApplication::SDLApplication() {
	if( !Application::instance ) 
		Application::instance =this;
	else {
		std::cerr << "SDLApplication: Multiple Application objects" << std::endl;
		return;
	}
	init();
	ShaderProgramCache::initialize();
}

SDLApplication::~SDLApplication() {
	ShaderProgramCache::cleanup();
}

bool SDLApplication::init() {
	if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
        std::cerr << "SDLApplication: Unable to initialize SDL" << std::endl;
        return false;
    }
    
    char *sbase =SDL_GetBasePath();
    if( !sbase )
	    m_baseDir = "./";
	else {
		m_baseDir = std::string( sbase );
		SDL_free( sbase );
	}	
		
    
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, REQUIRE_GL_MAJOR_VERSION);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, REQUIRE_GL_MINOR_VERSION);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_SHARE_WITH_CURRENT_CONTEXT, 1 );
	
	return true;
}

bool
SDLApplication::run() {
    
    /*for( WindowList::iterator it =window_list.begin(); it != window_list.end(); it++ )
			(*it)->create();*/
    
	running = true;

	uint32_t newtime,lasttime = SDL_GetTicks();
	float deltatime;

	while (running)
	{
		handleEvents();

		newtime = SDL_GetTicks();
		deltatime = (newtime - lasttime) * 0.001;
		
		for( WindowList::iterator it =window_list.begin(); it != window_list.end(); it++ )
			(*it)->update( deltatime );
		
		lasttime = newtime;
		
	}
	running =false;
	SDL_Quit();
	return true;
}

void SDLApplication::handleEvents()
{
	SDL_Event sdlevent;
	while(SDL_PollEvent(&sdlevent))
	{
		if (sdlevent.type == SDL_QUIT)
		{
			running = false;
		}
		else if (sdlevent.type == SDL_KEYDOWN)
		{
			if (sdlevent.key.keysym.sym == SDLK_ESCAPE)
			{
				running = false;
			}
			else
			{
				
				/* TODO: improve */
				char key = (char)sdlevent.key.keysym.sym ;
				KeyPressEvent e( KeyPressEvent::KEY_CHARACTER, key );
				emit( &e );
				
			}
		}
		else if (sdlevent.type == SDL_KEYUP)
		{

		}
		/*else if (sdlevent.type == SDL_VIDEORESIZE)
		{
		}*/
	}
}

void
SDLApplication::newWindow( Window* w) {	
}
