/*
 * This file is part of Folia, an experimental mini-engine using OpenGL 3
 * Created by Micky Faas. Freely usable and modifiable for academic purposes.
 */

#ifndef TENDRILTREE_H
#define TENDRILTREE_H

#include "../common/platform.h"
#include "../core/actor.h"
#include "../core/geometry.h"
#include "../utils/glm/vec3.hpp"
#include "tendrilfield.h"
#include <vector>

class TendrilTree : public Actor {
	public:
		TendrilTree( Object* parent );
		~TendrilTree();
		Geometry* geometry( int n =0 );
		
		virtual void update( float deltatime );
		
	private:
	
		void recursiveSubtree( Tendril& root, int depth, int maxDepth, int factor );
		glm::vec3 bezier2( float t, glm::vec3, glm::vec3, glm::vec3 );
	
		TendrilGeometry geom;
};

#endif