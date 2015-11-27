#include "tendriltree.h"
#include "../utils/shaderprogram.h"
#include "../common/shaderlayout.h"
#include "../utils/glm/gtc/random.hpp"
#include "../utils/glm/gtx/rotate_vector.hpp"
#include <stdio.h>

/* TendrilTree implementation */

TendrilTree::TendrilTree( Object* parent ) : Actor( parent ) {

	Tendril t;
	t.setAttrib( Tendril::SECTIONS, 8.0 );
	t.setAttrib( Tendril::THICKNESS, 1.0 );
	t.setAttrib( Tendril::CURL, 0.0 );
	t.setAttrib( Tendril::SHOULDER, glm::vec3( 0.0, 7.5, 0.1 ) );
	t.setAttrib( Tendril::HEAD, glm::vec3( 0.0, 15.0, 0.0 ) );
	//t.setAttrib( Tendril::GROWTH, 0.0 );
	
	geom.append( t );
	
	recursiveSubtree( t, 1, 3, 10 );
	geom.updateBuffers();
}

TendrilTree::~TendrilTree() {
}

Geometry* 
TendrilTree::geometry() {
	return &geom;
}

void 
TendrilTree::recursiveSubtree( Tendril& root, int depth, int maxDepth, int factor ) {

	if( depth > maxDepth )
		return;
		
	float t = 1.0;
	int first =0;
	glm::vec3 base = root.attribVector( Tendril::HEAD );

	for( int i =0; i < factor; i++ ) {
	
		Tendril tendril;
			
		//if( depth != maxDepth )
			tendril.setAttrib( Tendril::CURL, 0.0 );
			
		if( depth == 1 )
			tendril.setAttrib( Tendril::SECTIONS, 4.0 );
		else {
			tendril.setAttrib( Tendril::SECTIONS, 3.0 );
			tendril.setAttrib( Tendril::STEP, 0.5 );
		}
		
		/*glm::vec3 offset = glm::sphericalRand( 
			glm::length( root.attribVector( 
				Tendril::BASE ) - root.attribVector( Tendril::HEAD ) ) / ( 5 * t ) );*/
				
		glm::vec3 variation = glm::ballRand( 0.5 );
		
		glm::vec3 offset = glm::normalize( 
			root.attribVector( Tendril::HEAD ) - root.attribVector( Tendril::BASE ) )
			* glm::length( root.attribVector( 
					Tendril::BASE ) - root.attribVector( Tendril::HEAD ) ) / ( 5 * t );
		
		glm::vec3 axis = glm::sphericalRand( 1.0 );
		float angle = glm::linearRand( 0.5, 1.0 );
		
		offset = glm::rotate( offset, angle, axis );
		
		tendril.setAttrib( Tendril::BASE, base );
		tendril.setAttrib( Tendril::SHOULDER, base + offset * variation );
		tendril.setAttrib( Tendril::HEAD, base + offset * glm::vec3(2) );
		tendril.setAttrib( Tendril::THICKNESS, root.data[Tendril::THICKNESS] * 1.2 * (1.0f - t) );
		//tendril.setAttrib( Tendril::GROWTH, 0.0 );
		
		if( !first )
			first =geom.append( tendril );
		else
			geom.append( tendril );
		
		t = glm::linearRand(0.7f, 0.9f);
		base = bezier2( 
			t,
			root.attribVector( Tendril::BASE ),
			root.attribVector( Tendril::SHOULDER ),
			root.attribVector( Tendril::HEAD ) );
		
	}
	
	for( int i =0; i < factor; i++ ) {
	
		Tendril tendril = geom.dataAt(i);
		recursiveSubtree( tendril, depth+1, maxDepth, factor );
	}
}

void 
TendrilTree::update( float deltatime ) {
	for( int i =0; i < geom.tendrilCount(); i++ ) {

		Tendril &t = geom.dataAt(i);

		float g = t.data[Tendril::GROWTH];
		
		printf( "%f\n" , g );
		
		if( g < 1.0 ) {
			t.setAttrib( Tendril::GROWTH, g + 1.0 * deltatime );
			break;
		}
	}

	geom.updateBuffers();
	Actor::update( deltatime );
}

glm::vec3 
TendrilTree::bezier2(
  float t,
  glm::vec3 c1,
  glm::vec3 c2,
  glm::vec3 c3)
{
  return  powf( 1.0f-t, 2 ) * c1 + 2.0f*(1.0f-t)*t*c2 + powf( t, 2 ) * c3;
}