#include "tree_scene.h"
#include "utils/glm/gtc/random.hpp"
#include "utils/glm/gtx/rotate_vector.hpp"
#include "utils/glm/gtc/type_ptr.hpp"
#include <cstdio>

#include "core/camera.h"
#include "core/application.h"
#include "core/light.h"
#include "actors/solidcube.h"
#include "actors/ptree.h"
#include "actors/pointsprites.h"
#include "actors/plane.h"


TreeScene::TreeScene( Zone* parent, Camera *c ) : Actor( parent), camera( c ) {

	wire =false;
	
	tree = new PTree( parent );
	tree->transform().setPosition( glm::vec3(0) );
	
	halos = new PointSprites( this );
	halos->transform().setPosition( glm::vec3(0) );
	
	ground = new Plane( this );
	ground->transform().setPosition( glm::vec3(0) );
	ground->transform().setRotation( glm::vec3( M_PI / 2, 0.0f, 0.0f ) );
	ground->transform().setScale( 512.0f );
	
	skylight = new Light( parent );
	skylight->setIntensity( glm::vec3(1.0f, 0.9f, 0.9f) * 2.0f );
	skylight->setDirection( glm::vec3(0.1, -1.0, -0.2) );
	skylight->setType( Light::LIGHT_DIRECTIONAL );
	skylight->setVisible( false );
	parent->addLight( skylight );
	
	ShaderSource src;
	src.addSource( VERTEX_SHADER, "pointsprite" );
	src.addSource( GEOMETRY_SHADER, "pointsprite" );
	src.addSource( FRAGMENT_SHADER, "halo" );
	
	halos->setProgram( src.createProgram() );
	
	
	for( int i =0; i < 5; i++ ) {

		Light *l = new Light( parent );
		//l->setIntensity( glm::sphericalRand( 0.5f ) + glm::vec3(0.5) );
		l->setIntensity( glm::vec3(1.0f, 1.0f, 1.0f) );
		l->setDirection( glm::vec3(0) );
		l->setType( Light::LIGHT_POINT );
		l->setAttenuation( 0.09, 0.042 );
		l->transform().setPosition( glm::vec3( 0, 0, 5 ) );
		parent->addLight( l );
		
	//	SolidCube *cube =new SolidCube( this );
	//	cube->transform().setScale( 1.0f );
		
		int halo =halos->append( glm::vec3(0), 0.5f );
	
		Firefly f;
		f.light =l;
		f.halo =halo;
	//	f.cube =cube;
		f.position =glm::vec3(0.0,8.0,2.5) + glm::sphericalRand( 4.0f );
		f.phase =glm::linearRand( 0.0, M_PI );
		f.axis =glm::sphericalRand( 1.0f );
		
		fireflies.push_back( f );
		
	}
	
	up = glm::vec3(0,1,0);
	update( 1.0f );
	
}

TreeScene::~TreeScene(){}

void
TreeScene::update( float deltatime ) {

	theta += deltatime * 0.2;
	
	const glm::vec3 radius = glm::vec3( 1.5, 0, 0 );
	
	for( int i=0; i < fireflies.size(); i++ ) {
	
		Firefly &f =fireflies[i];
		
		glm::vec3 position =f.position + glm::rotate( radius, (float)M_PI * cosf( theta + f.phase ), f.axis );
		f.light->transform().setPosition( position );
		//f.cube->transform().setPosition( position );
		halos->setPosition( f.halo, position );
		
	}
	
	if( Application::app()->m_key ) {
		char key =Application::app()->m_key;
		Application::app()->m_key =0;
		switch( key ) {
			case 'w':
				wire =! wire;
				tree->setWireMode( wire );
				break;
				
			case 'e':
				toggleLights();
				break;
				
			case 'r':
				tree->reinitialize();
				break;
		}
	}

}

void
TreeScene::toggleLights() {

	bool fireflies_visible;
	if( skylight->isVisible() ) {
		skylight->setVisible( false );
		fireflies_visible =true;
	} else {
		skylight->setVisible( true );
		fireflies_visible =false;
	}
	
	for( int i=0; i < fireflies.size(); i++ ) {
		Firefly &f =fireflies[i];
		f.light->setVisible( fireflies_visible );
	}
	halos->setVisible( fireflies_visible );
}
