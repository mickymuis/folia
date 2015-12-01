#include "common/platform.h"
#include "core/sdlwindow.h"
#include "core/sdlapplication.h"
#include "core/engine.h"
#include "core/world.h"
#include "core/zone.h"
#include "core/camera.h"
#include "core/light.h"
#include "actors/solidcube.h"
#include "actors/tendrilfield.h"
#include "actors/tendriltree.h"
#include "utils/glm/vec3.hpp"


int main(int argc, char * argv[])
{
	SDLApplication app;
	SDLWindow window( "Folia" );
	//SDLWindow window2( "bleh" );
	app.add( &window );
	window.create();
	
	//window2.create();
	//app.add( &window2 );
	
	Engine engine;
	window.setEngine( &engine );
	//window2.setEngine( &engine );
	World w;
	Camera cam;
	cam.setPosition( glm::vec3(0,1,1));
	cam.setTarget( glm::vec3(0,0.5,0));
	cam.moveLocal( Camera::BACK, 25.0f );
	
	Camera cam2;
	//cam2.setPosition( glm::vec3( 0.0, 4.0, 5.0 ) );
	window.viewport()->world = &w;
	window.viewport()->camera = &cam;
	//window2.viewport()->world = &w;
	//window2.viewport()->camera = &cam2;
	
	SolidCube cube( w.currentZone() );
	cube.transform().setScale( 2.5 );
	SolidCube cube2( &cube );
	cube2.transform().setScale( 0.7 );
	SolidCube cube3( &cube2 );
	cube3.transform().setScale( 0.8 );
	cube.body().setRotationalVelocity( glm::vec3( 0.5, 0.0, 0.0 ) );
	cube2.transform().setPosition( glm::vec3( 4.0, 0.0, 0.0 ) );
	cube2.body().setRotationalVelocity( glm::vec3( 0.0, 1.0, 0.0 ) );
	cube3.transform().setPosition( glm::vec3( 7.0, 0.0, 0.0 ) );
	cube3.body().setAngularVelocity( glm::vec3( 0.0, 0.0, 2.0 ) );
	
	TendrilTree tendril ( w.currentZone() );
/*	TendrilTree tendril2 ( w.currentZone() );
	TendrilTree tendril3 ( w.currentZone() );

	tendril2.transform().translate( glm::vec3( -6.0, 0.0, -4.0 ) );
	tendril3.transform().translate( glm::vec3( 6.0, 0.0, -4.0 ) );
	tendril.body().setRotationalVelocity( glm::vec3( 0.0, 0.5, 0.0 ) );*/
	
	
	Light light0( w.currentZone() );
	light0.setIntensity( glm::vec3( 0.8 ) );
	light0.transform().setPosition( glm::vec3( 0, 5, 0 ) );
	w.currentZone()->addLight( &light0 );
	
	Light light1( w.currentZone() );
	light1.setIntensity( glm::vec3( 0,0.2,0.3 ) );
	light1.transform().setPosition( glm::vec3( 0, 0, 5 ) );
	w.currentZone()->addLight( &light1 );
	
	app.run();

	return 0;
}
