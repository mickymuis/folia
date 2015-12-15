/*
 * This file is part of Folia, an experimental mini-engine using OpenGL 3
 * Created by Micky Faas. Freely usable and modifiable for academic purposes.
 */

#ifndef APPLICATION_H
#define APPLICATION_H

#include "event.h"
#include <vector>
#include <string>

class Window;

class Application {
	public:
		Application() {};
		virtual ~Application() {}
		
		virtual bool run() =0;
		virtual bool isRunning() { return running; }
		void add( Window* w ) { window_list.push_back( w ); newWindow( w ); }
		
		virtual std::string resourceDir() const =0;
		
		static Application* app() { return Application::instance; }
		
		typedef std::vector<Window*> WindowList;
		typedef std::vector<EventHandler*> EventHandlerList;
		
		void attachEventHandler( EventHandler* );
		void detachEventHandler( EventHandler* );
		
		void emit( Event* e );
		
	protected:
		virtual void newWindow( Window* ) = 0;
		WindowList window_list;
		EventHandlerList handler_list;
		bool running;
		
		static Application* instance;
};

#endif