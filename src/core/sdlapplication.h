#ifndef SDLEVENTLOOP_H
#define SDLEVENTLOOP_H

#include "application.h"

class SDLApplication: public Application {
	public:
		SDLApplication();
	
		bool run();
		
	private:
		bool init();
		void handleEvents();
		void newWindow( Window* );

};

#endif