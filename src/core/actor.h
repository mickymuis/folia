#ifndef ACTOR_H
#define ACTOR_H

#include "../utils/glm/vec3.hpp"
#include "object.h"

class Geometry;

class Actor : public Object {
	public:
		enum {
			TYPE =0x3
		};
		Actor( Object *parent );
		~Actor();
		virtual int RTTI() const { return TYPE; }
		
		bool isVisible( ) const { return m_visible; }
		void setVisible( bool b ) { m_visible =b; }
		
		virtual Geometry* geometry() =0;
		
	private:
		bool m_visible;
};

#endif