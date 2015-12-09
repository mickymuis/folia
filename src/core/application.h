#ifndef APPLICATION_H
#define APPLICATION_H

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
		
		// HACK
		char m_key;

	protected:
		virtual void newWindow( Window* ) = 0;
		WindowList window_list;
		bool running;
		
		static Application* instance;
};

#endif