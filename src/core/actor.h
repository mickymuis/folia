#ifndef ACTOR_H
#define ACTOR_H

#include "../utils/glm/vec3.hpp"
#include "object.h"
#include "geometry.h"

class Body {
	public:
		Body();
		
		void setLinearVelocity( glm::vec3 v ) { m_v_lin = v; }
		glm::vec3 linearVelocity() { return m_v_lin; }
		
		void setAngularVelocity( glm::vec3 v ) { m_v_ang = v; }
		glm::vec3 angularVelocity() { return m_v_ang; }
		
		void setRotationalVelocity( glm::vec3 v ) { m_v_rot = v; }
		glm::vec3 rotationalVelocity() { return m_v_rot; }
		
		void setMass( float m ) { m_mass =m; }
		float mass() { return m_mass; }
		
		void update( float deltatime, Transformation& );
		
	private:
		glm::vec3 m_v_lin;
		glm::vec3 m_v_ang;
		glm::vec3 m_v_rot;
		float m_mass;
};

class Actor : public Object {
	public:
		enum {
			TYPE =0x3
		};
		Actor( Object *parent );
		~Actor();
		virtual int RTTI() const { return TYPE; }
		
		virtual void update( float deltatime );
		
		bool isVisible( ) const { return m_visible; }
		void setVisible( bool b ) { m_visible =b; }
		
		Body& body() { return m_body; }
		
		virtual Geometry* geometry() =0;
		
	private:
		bool m_visible;
		Body m_body;
};

#endif