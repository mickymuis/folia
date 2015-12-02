#include "zone.h"
#include <algorithm>

Zone::Zone( World* world ) : w(0) {
	if( world )
		world->addZone( this );
}

Zone::~Zone() {
	if( w )
		w->removeZone( this );
}

void Zone::addLight( Light* l ) {
	m_lights.push_back( l );
}

void Zone::removeLight( Light* l ) {
	std::vector<Light*>::iterator it = std::find( m_lights.begin(), m_lights.end(), l );
	if( it != m_lights.end() )
		m_lights.erase( it );
}
