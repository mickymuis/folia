#include "tree_scene.h"
#include "utils/glm/gtc/random.hpp"
#include "utils/glm/gtx/rotate_vector.hpp"
#include "utils/glm/gtc/type_ptr.hpp"

TreeScene::TreeScene( Zone* parent, Camera *c ) : Actor( parent), camera( c ) {

	tree = new PTree( parent );
	tree->transform().setPosition( glm::vec3(0) );
	
	for( int i =0; i < 10; i++ ) {

		Light *l = new Light( parent );
		//l->setIntensity( glm::sphericalRand( 0.5f ) + glm::vec3(0.5) );
		l->setIntensity( glm::vec3(0.6f, 0.8f, 0.6f) );
		l->setDirection( glm::vec3(0) );
		l->setType( Light::LIGHT_POINT );
		l->setAttenuation( 0.0014, 0.0007 );
		l->transform().setPosition( glm::vec3( 0, 0, 5 ) );
		parent->addLight( l );
		
		SolidCube *cube =new SolidCube( this );
		cube->transform().setScale( 0.1f );
	
		Firefly f;
		f.light =l;
		f.cube =cube;
		f.position =glm::vec3(0.0,60.0,0.0) + glm::sphericalRand( 50.0f );
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

	theta += deltatime * 0.5;
	
	const glm::vec3 radius = glm::vec3( 10, 0, 0 );
	
	for( int i=0; i < fireflies.size(); i++ ) {
	
		Firefly &f =fireflies[i];
		
		glm::vec3 position =f.position + glm::rotate( radius, (float)M_PI * cosf( theta + f.phase ), f.axis );
		f.light->transform().setPosition( position );
		f.cube->transform().setPosition( position );
		
	}

}