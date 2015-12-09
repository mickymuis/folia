#ifndef TREESCENE_H
#define TREESCENE_H

#include "core/engine.h"
#include "core/actor.h"
#include "core/zone.h"
#include "utils/glm/vec3.hpp"
#include <vector>

class PointSprites;
class PTree;
class Camera;
class Light;
class SolidCube;
class Plane;

class TreeScene : public Actor {
	public:
	
		TreeScene( Zone* parent, Camera* );
		~TreeScene();
		
		Geometry* geometry ( int ) { return 0; };
		void update( float deltatime );
		
	private:
		Camera *camera;
		PTree *tree;
		PointSprites *halos;
		Plane *ground;
		Plane *sky;
		Light *skylight;
		
		void toggleLights();
		
		class Firefly {
			public:
				Light *light;
				SolidCube *cube;
				int halo;
				glm::vec3 position;
				glm::vec3 axis;
				
				float phase;
		};
		
		float theta;
		bool wire;
		
		std::vector<Firefly> fireflies;
		glm::vec3 up;
	
};

#endif
