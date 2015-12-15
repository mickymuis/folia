/*
 * This file is part of Folia, an experimental mini-engine using OpenGL 3
 * Created by Micky Faas. Freely usable and modifiable for academic purposes.
 */

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

	setEventFilters( EVENT_KEYPRESS );
	Application::app()->attachEventHandler( this );
	wire =false;
	
	setupTree();
	
	halos = new PointSprites( this );
	halos->transform().setPosition( glm::vec3(0) );
	
	ground = new Plane( this );
	ground->transform().setPosition( glm::vec3(0) );
	ground->transform().setRotation( glm::vec3( M_PI / 2, 0.0f, 0.0f ) );
	ground->transform().setScale( 512.0f );
	
	skylight = new Light( parent );
	skylight->setIntensity( glm::vec3(1.2f, 0.8f, 0.75f) * 2.0f );
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
		
		int halo =halos->append( glm::vec3(0), 0.5f );
	
		Firefly f;
		f.light =l;
		f.halo =halo;
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
		halos->setPosition( f.halo, position );
	}
		
}

void 
TreeScene::event( Event* e ) {
	
	if( e->type() == EVENT_KEYPRESS ) {
		KeyPressEvent* ke =(KeyPressEvent*)e;
	
		switch( ke->getChar() ) {
			case 'w':
				wire =! wire;
				tree->setWireMode( wire );
				ke->accept();
				break;
			
			case 'e':
				toggleLights();
				ke->accept();
				break;
			
			case 'r':
				tree->reinitialize();
				ke->accept();
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

void
TreeScene::setupTree() {
	tree = new PTree( parent );
	tree->transform().setPosition( glm::vec3(0) );
	
	PTree::Constraint constraints[4];
	
	/* These are the per-branch-level parameters of the algorithm */
	constraints[0].max_extensions = 12;	
	constraints[0].min_extensions = 11;	
	constraints[0].sections =12;	
	constraints[0].steps =3;
	constraints[0].max_radius =1.0;
	constraints[0].min_radius =0.5;
	constraints[0].max_length =1.8;
	constraints[0].min_length =1.0;
	constraints[0].max_total_length =15.0;
	constraints[0].max_curvature =0.5;				
	constraints[0].max_rough =0.7;
	constraints[0].max_growth_length =0.8;
	constraints[0].max_growth_radius =0.04;
	constraints[0].min_branch_distance =5;
	
	constraints[1].max_extensions = 8;
	constraints[1].min_extensions = 5;	
	constraints[1].sections =8;
	constraints[1].steps =4;
	constraints[1].max_radius =0.3;
	constraints[1].min_radius =0.1;
	constraints[1].max_length =1.0;
	constraints[1].min_length =0.5;
	constraints[1].max_total_length =7.0;
	constraints[1].max_curvature =0.15;				
	constraints[1].max_rough =0.5;
	constraints[1].max_growth_length =0.4;
	constraints[1].max_growth_radius =0.04;
	constraints[1].min_branch_distance =2;
	
	constraints[2].max_extensions = 3;	
	constraints[2].min_extensions = 2;	
	constraints[2].sections =6;		
	constraints[2].steps =3;
	constraints[2].max_radius =0.15;
	constraints[2].min_radius =0.08;
	constraints[2].max_length =0.4;
	constraints[2].min_length =0.2;
	constraints[2].max_total_length =3.0;
	constraints[2].max_curvature =0.1;				
	constraints[2].max_rough =0.0;
	constraints[2].max_growth_length =0.4;
	constraints[2].max_growth_radius =0.04;
	constraints[2].min_branch_distance =1;
	
	constraints[3].max_extensions = 1;	
	constraints[3].min_extensions = 1;	
	constraints[3].sections =3;		
	constraints[3].steps =3;
	constraints[3].max_radius =0.08;
	constraints[3].min_radius =0.05;
	constraints[3].max_length =0.4;
	constraints[3].min_length =0.2;
	constraints[3].max_total_length =3.0;
	constraints[3].max_curvature =0.05;				
	constraints[3].max_rough =0.0;
	constraints[3].max_growth_length =0.4;
	constraints[3].max_growth_radius =0.04;
	constraints[3].min_branch_distance =1;
	
	for( int i =0; i < 4; i++ )
		tree->setConstraint( i, constraints[i] );
		
	tree->setBranchDepth( 4 );
	tree->reinitialize();
}
