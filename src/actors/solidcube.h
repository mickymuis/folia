#ifndef SOLIDCUBE_H
#define SOLIDCUBE_H

#include "../common/platform.h"
#include "../utils/shaderprogram.h"
#include "../core/actor.h"
#include "../core/geometry.h"

class SolidCubeGeometry : public Geometry {
	public:
		SolidCubeGeometry();
		~SolidCubeGeometry();
		
	private:
		void initBuffers();
		void destroyBuffers();
		
		GLuint vbo_position;
		GLuint vbo_normal;
		ShaderProgram m_prog;
	
};

class SolidCube : public Actor {
	public:
		SolidCube( Object* parent );
		~SolidCube();
		Geometry* geometry();
		
	private:
		SolidCubeGeometry cube;
};

#endif