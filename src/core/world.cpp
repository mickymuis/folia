#include "world.h"
#include "zone.h"

World::World( ) {
	m_default =new Zone( this );
	setCurrentZone( m_default );
}
World::~World( ) {
	Zones cpy =m_zones;
	for( Zones::iterator it =cpy.begin(); it != cpy.end(); it++ ) {
		if( (*it) != m_default )
			delete (*it);
	}
	delete m_default;
}

void World::update( uint32_t deltatime, bool onlyCurrent ) {

	if( onlyCurrent ) {
		if( m_current )
			m_current->update( deltatime );
	} else {
		for( Zones::iterator it =m_zones.begin(); it != m_zones.end(); it++ ) 
			(*it)->update( deltatime );
	}
}
			
void World::addZone( Zone* z ) {
	m_zones.insert( z );
	z->w = this;
}

void World::removeZone( Zone* z ) {

	if( m_zones.erase( z ) )
		z->w =0;
}

void World::setCurrentZone( Zone* z ) {
	m_zones.insert( z );
	m_current = z;
}

Zone* World::currentZone() const {
	return m_current;
}