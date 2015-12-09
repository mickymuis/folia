#ifndef WINDOW_H
#define WINDOW_H

#define WINDOW_DEFAULT_WIDTH 1024
#define WINDOW_DEFAULT_HEIGHT 768

#include <cinttypes>
#define GLM_SWIZZLE
#include "../utils/glm/vec4.hpp"

class Engine;
class World;
class Camera;

class Viewport {
	public:
		Viewport() : 
			width(0), height(0), x(0), y(0), world(0), camera(0), background( glm::vec4( 0.3f, 0.3f, 0.3f, 1.0f ) ) {}
		
		int width, height, x, y;
		float aspect;
		World *world;
		Camera *camera;
		glm::vec4 background;
};

class Window {
	public:
		enum ViewportPosition {
			VP_DEFAULT
		};
		
		Window( const char* title );
		virtual ~Window() {}
		
		Viewport *viewport( ViewportPosition p= VP_DEFAULT );
		int viewportCount() { return 1; } // for now
		
		virtual void create() =0;
		virtual void update( float deltatime ) =0;
		
		virtual void setEngine( Engine* e ) { engine = e; }

	protected:
		void updateViewportDimensions( ViewportPosition p= VP_DEFAULT );
	
		Viewport viewports[1];
		Engine *engine;
		const char *title;
		
		int width, height;
};

#endif