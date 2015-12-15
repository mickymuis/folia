/*
 * This file is part of Folia, an experimental mini-engine using OpenGL 3
 * Created by Micky Faas. Freely usable and modifiable for academic purposes.
 */

#ifndef TREESCENE_H
#define TREESCENE_H

#include "core/engine.h"
#include "core/actor.h"
#include "core/zone.h"
#include "core/event.h"
#include "utils/glm/vec3.hpp"
#include <vector>

class PointSprites;
class PTree;
class Camera;
class Light;
class SolidCube;
class Plane;

class TreeScene : public Actor, public EventHandler {
	public:
	
		TreeScene( Zone* parent, Camera* );
		~TreeScene();
		
		Geometry* geometry ( int ) { return 0; };
		void update( float deltatime );
		
		void event( Event* e );
		
	private:
		Camera *camera;
		PTree *tree;
		PointSprites *halos;
		Plane *ground;
		Plane *sky;
		Light *skylight;
		
		void toggleLights();
		void setupTree();
		
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
