#ifndef GEOMETRY_H
#define GEOMETRY_H

#include "../utils/platform.h"

class Geometry {
	public:
		Geometry() {}
		~Geometry() {}
		
		virtual GLuint vao() const { return m_vao; }
		virtual GLuint program() const { return m_program; }
		virtual GLenum type() const { return m_type; }
		virtual GLint first() const { return 0; }
		virtual GLsizei size() const { return m_size; } 
		
	protected:
		GLuint m_vao;
		GLuint m_program;
		GLenum m_type;
		GLsizei m_size;
};

#endif