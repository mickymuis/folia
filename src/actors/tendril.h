#ifndef TENDRIL_H
#define TENDRIL_H

#include "../common/platform.h"
#include "../core/actor.h"
#include "../core/geometry.h"
#include "../utils/glm/vec3.hpp"

class TendrilGeometry : public Geometry {
	public:
		TendrilGeometry();
		~TendrilGeometry();
		
		void setPosition( int, glm::vec3 );
		void updateBuffers();
		
	private:
		void initBuffers();
		void destroyBuffers();
		
		GLuint vbo_position;
		GLuint vbo_normal;
		
		GLfloat vertices[9];
};

class Tendril : public Actor {
	public:
		Tendril( Object* parent );
		~Tendril();
		Geometry* geometry();
		
	private:
		TendrilGeometry geom;
};

#endif