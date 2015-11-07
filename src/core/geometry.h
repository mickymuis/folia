#ifndef GEOMETRY_H
#define GEOMETRY_H

#include "../common/platform.h"
#include "../utils/shaderprogram.h"
#include "../utils/glm/fwd.hpp"
#include "../utils/glm/mat4x4.hpp"
#include "../utils/glm/vec3.hpp"

class Transformation {

	public:
		Transformation();
		
		/* Absolute functions */
		void setPosition( glm::vec3 p ) { m_position = p;  m_uptodate =false; }
		glm::vec3 position() const { return m_position; }
		
		void setRotation( glm::vec3 r ) { m_rotation = r; m_uptodate =false; }
		glm::vec3 rotation() const { return m_rotation; }
		
		void setScale( float s ) { m_scale = s; m_uptodate =false; }
		float scale() const { return m_scale; };
		
		void setOrbit( glm::vec3 o ) { m_orbit = o; m_uptodate =false; }
		glm::vec3 orbit() const { return m_orbit; }
		
		/* Relative utility functions */
		void translate( glm::vec3 t ) { m_position += t; m_uptodate =false; }
		void rotate( glm::vec3 r ) { m_rotation += r; m_uptodate =false; }
		void rotateAround( glm::vec3 r ) { m_orbit += r; m_uptodate =false; }
		
		/* Matrix generation */
		glm::mat4 matrix();
	
	private:
		glm::vec3 m_position;
		glm::vec3 m_rotation;
		float m_scale;
		glm::vec3 m_orbit;
		glm::mat4 m_matCache;
		bool m_uptodate;
	
};

class Geometry {
	public:
		Geometry() {}
		~Geometry() {}
		
		virtual GLuint vao() const { return m_vao; }
		virtual ShaderProgram* program() const { return m_program; }
		virtual GLenum type() const { return m_type; }
		virtual GLint first() const { return 0; }
		virtual GLsizei size() const { return m_size; } 
		
	protected:
		GLuint m_vao;
		ShaderProgram* m_program;
		GLenum m_type;
		GLsizei m_size;
};

#endif