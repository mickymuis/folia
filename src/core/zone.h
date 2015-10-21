#ifndef ZONE_H
#define ZONE_H

#include "object.h"
#include "world.h"
#include <set>

class Light;

class Zone : public Object {
	public:
		enum {
			TYPE =0x4
		};
		Zone( World* world );
		~Zone();
		
		void addLight( Light* );
		void removeLight( Light* );
		std::set<Light*> lights() const { return m_lights; }
		
		virtual int RTTI() const { return TYPE; }
		
	private:
		std::set<Light*> m_lights;
	
	protected:
		friend class World;
		World* w;

};

#endif