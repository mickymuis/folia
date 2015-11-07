#ifndef SDLEVENTLOOP_H
#define SDLEVENTLOOP_H

#include "application.h"

class SDLApplication: public Application {
	public:
		SDLApplication();
		~SDLApplication();
	
		bool run();
		std::string resourceDir() const { return m_baseDir; }
		
	private:
		bool init();
		void handleEvents();
		void newWindow( Window* );
		
		std::string m_baseDir;

};

#endif