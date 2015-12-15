/*
 * This file is part of Folia, an experimental mini-engine using OpenGL 3
 * Created by Micky Faas. Freely usable and modifiable for academic purposes.
 */

#include "geometry.h"

#include "../utils/glm/glm.hpp"
#include "../utils/glm/gtc/type_ptr.hpp"
#include "../utils/glm/gtc/matrix_transform.hpp"
#include "../utils/glm/gtx/transform.hpp"

Transformation::Transformation() 
	: m_position( 0.0, 0.0, 0.0 ),
	  m_rotation( 0.0, 0.0, 0.0 ),
	  m_scale( 1.0 ),
	  m_orbit ( 0.0, 0.0, 0.0 ),
	  m_matCache ( 1.0 ),
	  m_uptodate ( false )
	{}

glm::mat4 
Transformation::matrix() {

	if( !m_uptodate ) {
		glm::mat4 mat( 1.0 );
			
		mat =glm::scale( mat, glm::vec3( m_scale ) );
		mat =glm::rotate( mat, 
			m_orbit.x, glm::vec3( 1.0, 0.0, 0.0 ) );
		mat =glm::rotate( mat, 
			 m_orbit.y, glm::vec3( 0.0, 1.0, 0.0 ) );
		mat =glm::rotate( mat, 
			m_orbit.z, glm::vec3( 0.0, 0.0, 1.0 ) );
		
		mat =glm::translate( mat, m_position );
		mat =glm::rotate( mat, 
			m_rotation.x, glm::vec3( 1.0, 0.0, 0.0 ) );
		mat =glm::rotate( mat, 
			m_rotation.y, glm::vec3( 0.0, 1.0, 0.0 ) );
		mat =glm::rotate( mat, 
			m_rotation.z, glm::vec3( 0.0, 0.0, 1.0 ) );
			
		m_matCache =mat;

		m_uptodate =true;
	}
	return m_matCache;
}