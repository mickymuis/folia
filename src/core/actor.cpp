#include "actor.h"
#include "../utils/glm/glm.hpp"

Body::Body() 
	: m_v_lin ( 0.0 ),
	  m_v_ang ( 0.0 ),
	  m_v_rot ( 0.0 ),
	  m_mass ( 1.0 )
	{}

void 
Body::update( float deltatime, Transformation& t ) {

	t.translate( deltatime * m_v_lin );
	t.rotate( deltatime * m_v_rot );
	t.rotateAround( deltatime * m_v_ang );
}

Actor::Actor( Object *parent )  : Object( parent ), m_visible( true ) {

}

Actor::~Actor() {}

void
Actor::update( float deltatime ) {
	m_body.update( deltatime, transform() );
	Object::update( deltatime );
}