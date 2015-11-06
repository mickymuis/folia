#include "sdlapplication.h"
#include "../common/platform.h"
#include "window.h"
#include <iostream>
#include <stdio.h>

Applcation*
Application::instance =0;

SDLApplication::SDLApplication() {
	if( !Application::instance ) 
		Application::instance =this;
	else {
		std::cerr << "SDLApplication: Multiple Application objects" << std::endl;
		return;
	}
	init();
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
		m_baseDir = string( sbase );
		SDL_Free( sbase );
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
/*	int i;
	for (i = 0; i < SDL_NUM_SCANCODES; ++i)
		inputstate.keyspress[i] = false;*/

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
				//inputstate.keyspress[sdlevent.key.keysym.sym] = true;
				//inputstate.keysdown[sdlevent.key.keysym.sym] = true;
			}
		}
		else if (sdlevent.type == SDL_KEYUP)
		{
			//inputstate.keysdown[sdlevent.key.keysym.sym] = false;
		}
		/*else if (sdlevent.type == SDL_VIDEORESIZE)
		{
			width = sdlevent.resize.w;
			height = sdlevent.resize.h;
		}*/
	}
}

void
SDLApplication::newWindow( Window* w) {	
}