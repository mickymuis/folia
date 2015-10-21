#ifndef LIGHT_H
#define LIGHT_H

#include "zone.h"
#include "../utils/glm/vec3.hpp"

class Light : public Object {
	public:
		enum {
			TYPE =0x6
		};
		Light( Zone* parent ) : Object( parent ) {}
		~Light() {}
		
		void setIntensity( glm::vec3 i ) { m_intensity =i; }
		glm::vec3 intensity() const { return m_intensity; }
		
		virtual int RTTI() const { return TYPE; }
		
	private:
		glm::vec3 m_intensity;
};

#endif