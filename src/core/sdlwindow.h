#ifndef SDLWINDOW_H
#define SDLWINDOW_H

#include "window.h"
#include "sdlapplication.h"

class SDLWindow : public Window
{
public:
	SDLWindow( const char* );
	~SDLWindow();

	void create();
	void update( uint32_t deltatime );

private:
	friend class SDLApplication;
	SDL_Window *window;
	SDL_GLContext glcontext;
	
	bool hasContext, hasWindow;
	
	bool createWindow();
	void cleanup();
};

#endif