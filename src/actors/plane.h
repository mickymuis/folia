#ifndef PLANE_H
#define PLANE_H

#include "../common/platform.h"
#include "../core/actor.h"
#include "../core/geometry.h"
#include "../utils/glm/vec3.hpp"
#include "../utils/glm/vec4.hpp"
#include "../utils/glm/vec2.hpp"
#include <vector>

class PlaneGeometry : public Geometry {
	public:
		struct Point {
			glm::vec3 position;
			glm::vec3 normal;
			glm::vec4 color;
			glm::vec2 texcoords;
			glm::vec3 tangent;
		};
		
		PlaneGeometry();
		~PlaneGeometry();
		
		virtual bool renderDeferred( int n = 0 ) const { return true; }
		virtual GLsizei size( int n = 0 ) const { return 4; } 
		
		void setProgram( ShaderProgram* );
		void updateBuffers();
			
	private:
		void initBuffers();
		void destroyBuffers();
		
		GLuint vbo_points;
		
		Point m_data[4];
};

class Plane : public Actor {
	public:
		Plane( Object* parent );
		~Plane();
		
		void setProgram( ShaderProgram* );
		
		Geometry* geometry( int n =0 );
		
	private:
		PlaneGeometry geom;
};

#endif