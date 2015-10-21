#include "sdlapplication.h"
#include "../utils/platform.h"
#include "window.h"
#include <iostream>
#include <stdio.h>

SDLApplication::SDLApplication() {
	init();
}

bool SDLApplication::init() {
	if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
        std::cerr << "SDLApplication: Unable to initialize SDL" << std::endl;
        return false;
    }
    
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_SHARE_WITH_CURRENT_CONTEXT, 1 );
	
	return true;
}

bool
SDLApplication::run() {
    
    /*for( WindowList::iterator it =window_list.begin(); it != window_list.end(); it++ )
			(*it)->create();*/
    
	running = true;

	uint32_t lasttime = SDL_GetTicks();
	uint32_t newtime, deltatime;

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