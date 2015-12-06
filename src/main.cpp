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
	cam.setPosition( glm::vec3(0.1, 4.0,20));
	cam.setTarget( glm::vec3(-0.1, 8.0,-0.1));
	//cam.moveLocal( Camera::BACK, 25.0f );

	window.viewport()->world = &w;
	window.viewport()->camera = &cam;
	//window2.viewport()->world = &w;
	//window2.viewport()->camera = &cam2;
	
	/*SolidCube cube( w.currentZone() );
	cube.transform().setPosition( glm::vec3( 0, 0, 0 ) );
	cube.transform().setScale( 2.5 );
	SolidCube cube2( &cube );
	cube2.transform().setScale( 0.7 );
	SolidCube cube3( &cube2 );
	cube3.transform().setScale( 0.8 );
	cube.body().setRotationalVelocity( glm::vec3( 0.5, 0.0, 0.0 ) );
	cube2.transform().setPosition( glm::vec3( 4.0, 0.0, 0.0 ) );
	cube2.body().setRotationalVelocity( glm::vec3( 0.0, 1.0, 0.0 ) );
	cube3.transform().setPosition( glm::vec3( 7.0, 0.0, 0.0 ) );
	cube3.body().setAngularVelocity( glm::vec3( 0.0, 0.0, 2.0 ) );*/
	
/*	TendrilTree tendril ( w.currentZone() );
	tendril.transform().setPosition( glm::vec3( 0, 0, 0 ) );
	TendrilTree tendril2 ( w.currentZone() );
	TendrilTree tendril3 ( w.currentZone() );

	tendril2.transform().translate( glm::vec3( -6.0, 0.0, -4.0 ) );
	tendril3.transform().translate( glm::vec3( 6.0, 0.0, -4.0 ) );
	tendril.body().setRotationalVelocity( glm::vec3( 0.0, 0.5, 0.0 ) );*/
	
	//PTree tree ( w.currentZone() );
	//tree.transform().setPosition( glm::vec3( 0, 0, 0 ) );
//	PTree tree2 ( w.currentZone() );
//	tree2.transform().setPosition( glm::vec3( -2, 0, 2 ) );
	//tree.body().setRotationalVelocity( glm::vec3( 0.0, 0.5, 0.0 ) );
	
/*	Light light0( w.currentZone() );
	light0.setIntensity( glm::vec3( 0.8 ) );
	light0.setDirection( glm::vec3(0,-1,-0.2) );
	light0.setType( Light::LIGHT_DIRECTIONAL );
	light0.setAttenuation( 0.045, 0.0075 );
	light0.transform().setPosition( glm::vec3( 0, 15, 0 ) );
	w.currentZone()->addLight( &light0 );
	
	Light light1( w.currentZone() );
	light1.setIntensity( glm::vec3( 0,0.2,0.3 ) );
	light1.setDirection( glm::vec3(0) );
	light1.setType( Light::LIGHT_POINT );
	light1.setAttenuation( 0.045, 0.0075 );
	light1.transform().setPosition( glm::vec3( 0, 0, 5 ) );
	w.currentZone()->addLight( &light1 );*/
	
	TreeScene scene( w.currentZone(), &cam );
	
	app.run();

	return 0;
}
