#include "utils/platform.h"
#include "core/sdlwindow.h"
#include "core/sdlapplication.h"
#include "core/engine.h"
#include "core/world.h"
#include "core/zone.h"
#include "core/camera.h"
#include "actors/solidcube.h"
#include "utils/glm/vec3.hpp"


int main(int argc, char * argv[])
{
	SDLApplication app;
	SDLWindow window( "Folia" );
	SDLWindow window2( "bleh" );
	app.add( &window );
	window.create();
	
	window2.create();
	app.add( &window2 );
	
	Engine engine;
	window.setEngine( &engine );
	window2.setEngine( &engine );
	World w;
	Camera cam;
	Camera cam2;
	cam2.setPosition( glm::vec3( 0.0, 4.0, 5.0 ) );
	window.viewport()->world = &w;
	window.viewport()->camera = &cam;
	window2.viewport()->world = &w;
	window2.viewport()->camera = &cam2;
	
	SolidCube cube( w.currentZone() );
	SolidCube cube2( w.currentZone() );
	SolidCube cube3( &cube2 );
	cube2.setPosition( glm::vec3( 3.0, 0.0, 0.0 ) );
	cube3.setPosition( glm::vec3( 3.0, 0.0, 0.0 ) );
	
	app.run();

	return 0;
}
