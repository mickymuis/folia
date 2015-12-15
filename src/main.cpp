/*
 * This file is part of Folia, an experimental mini-engine using OpenGL 3
 * Created by Micky Faas. Freely usable and modifiable for academic purposes.
 */

#include "common/platform.h"
#include "core/sdlwindow.h"
#include "core/sdlapplication.h"
#include "core/engine.h"
#include "core/world.h"
#include "core/zone.h"
#include "core/camera.h"
#include "tree_scene.h"

int main(int argc, char * argv[])
{
	/* We create the bare necessary stuff to run */
	SDLApplication app;
	SDLWindow window( "Folia" );
	
	app.add( &window );
	window.create();
	
	Engine engine;
	window.setEngine( &engine );

	World w;
	Camera cam;
	cam.setPosition( glm::vec3(0.1, 4.0,20));
	cam.setTarget( glm::vec3(-0.1, 8.0,-0.1));

	/* Viewport is a simple structure that holds a Window's contents */
	window.viewport()->world = &w;
	window.viewport()->camera = &cam;
	
	/* Finally, we put our `scene' into the world and let it populate world further */
	TreeScene scene( w.currentZone(), &cam );
	scene.transform().setPosition( glm::vec3(0) );
	
	app.run();

	return 0;
}
