#ifndef APPLICATION_H
#define APPLICATION_H

#include <vector>

class Window;

class Application {
	public:
		Application() {};
		virtual ~Application() {}
		
		virtual bool run() =0;
		virtual bool isRunning() { return running; }
		void add( Window* w ) { window_list.push_back( w ); newWindow( w ); }
		
		typedef std::vector<Window*> WindowList;

	protected:
		virtual void newWindow( Window* ) = 0;
		WindowList window_list;
		bool running;
};

#endif