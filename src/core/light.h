/*
 * This file is part of Folia, an experimental mini-engine using OpenGL 3
 * Created by Micky Faas. Freely usable and modifiable for academic purposes.
 */

#ifndef LIGHT_H
#define LIGHT_H

#include "zone.h"
#include "../utils/glm/vec3.hpp"

class Light : public Object {
	public:
		enum {
			TYPE =0x6
		};
		enum LightType {
			LIGHT_DIRECTIONAL,
			LIGHT_POINT
		};
		Light( Zone* parent ) : Object( parent ), m_visible( true ) {}
		~Light() {}
		
		void setType( LightType t ) { m_type =t; }
		LightType type() const { return m_type; }
		
		void setIntensity( glm::vec3 i ) { m_intensity =i; }
		glm::vec3 intensity() const { return m_intensity; }
		
		void setDirection( glm::vec3 d ) { m_direction =d; }
		glm::vec3 direction() const { return m_direction; }
		
		void setAttenuation( float linear, float quadratic ) { m_linear =linear; m_quadratic =quadratic; }
		float linearAttenuation() const { return m_linear; }
		float quadraticAttenuation() const { return m_quadratic; }
		
		void setVisible( bool b ) { m_visible =b; }
		bool isVisible( ) const { return m_visible; }
		
		virtual int RTTI() const { return TYPE; }
		
	private:
		LightType m_type;
		
		glm::vec3 m_intensity;
		glm::vec3 m_direction;
		float m_linear;
		float m_quadratic;
		bool m_visible;
};

#endif