/*
 * This file is part of Folia, an experimental mini-engine using OpenGL 3
 * Created by Micky Faas. Freely usable and modifiable for academic purposes.
 */

#ifndef ZONE_H
#define ZONE_H

#include "object.h"
#include "world.h"
#include <vector>

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
		std::vector<Light*> lights() const { return m_lights; }
		
		virtual int RTTI() const { return TYPE; }
		
	private:
		std::vector<Light*> m_lights;
	
	protected:
		friend class World;
		World* w;

};

#endif