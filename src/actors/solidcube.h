/*
 * This file is part of Folia, an experimental mini-engine using OpenGL 3
 * Created by Micky Faas. Freely usable and modifiable for academic purposes.
 */

#ifndef SOLIDCUBE_H
#define SOLIDCUBE_H

#include "../common/platform.h"
#include "../core/actor.h"
#include "../core/geometry.h"

class SolidCubeGeometry : public Geometry {
	public:
		SolidCubeGeometry();
		~SolidCubeGeometry();
		
		virtual bool renderDeferred( int n = 0 ) const { return true; }
		
	private:
		void initBuffers();
		void destroyBuffers();
		
		GLuint vbo_position;
		GLuint vbo_normal;
};

class SolidCube : public Actor {
	public:
		SolidCube( Object* parent );
		~SolidCube();
		Geometry* geometry( int n =0 );
		
	private:
		SolidCubeGeometry cube;
};

#endif