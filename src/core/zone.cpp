#include "zone.h"

Zone::Zone( World* world ) : w(0) {
	if( world )
		world->addZone( this );
}

Zone::~Zone() {
	if( w )
		w->removeZone( this );
}

void Zone::addLight( Light* l ) {
	m_lights.insert( l );
}

void Zone::removeLight( Light* l ) {
	m_lights.erase( l );
}