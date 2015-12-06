#ifndef TREESCENE_H
#define TREESCENE_H

#include "core/engine.h"
#include "core/world.h"
#include "core/zone.h"
#include "core/camera.h"
#include "core/light.h"
#include "actors/solidcube.h"
#include "actors/tendrilfield.h"
#include "actors/tendriltree.h"
#include "actors/ptree.h"
#include "utils/glm/vec3.hpp"
#include <vector>

class TreeScene : public Actor {
	public:
	
		TreeScene( Zone* parent, Camera* );
		~TreeScene();
		
		Geometry* geometry ( int ) { return 0; };
		void update( float deltatime );
		
	private:
		Camera *camera;
		PTree *tree;
		
		class Firefly {
			public:
				Light *light;
				SolidCube *cube;
				glm::vec3 position;
				glm::vec3 axis;
				
				float phase;
		};
		
		float theta;
		
		std::vector<Firefly> fireflies;
		glm::vec3 up;
	
};

#endif
