/*
 * This file is part of Folia, an experimental mini-engine using OpenGL 3
 * Created by Micky Faas. Freely usable and modifiable for academic purposes.
 */

#ifndef WORLD_H
#define WORLD_H

#include <cinttypes>
#include <set>

class Zone;

class World {
	public:
		World( );
		~World( );
		
		typedef std::set<Zone*> Zones;
		
		void update( float deltatime, bool onlyCurrent =true );
		
		void addZone( Zone* );
		void removeZone( Zone* );
		void setCurrentZone( Zone* );
		Zone* currentZone() const;
		Zone* defaultZone() const { return m_default; }
		
	private:
		Zones m_zones;
		Zone* m_default;
		Zone* m_current;
};

#endif