#include "../common/platform.h"
#include "engine.h"
#include "window.h"
#include "zone.h"
#include "world.h"
#include "actor.h"
#include "geometry.h"
#include "camera.h"
#include "object.h"
#include "../utils/shaderprogram.h"
#include "../utils/glm/glm.hpp"
#include "../utils/glm/gtc/type_ptr.hpp"
#include "../utils/glm/gtc/matrix_transform.hpp"
#include "../utils/glm/gtx/transform.hpp"
#include "../actors/solidcube.h"
#include <stack>
#include <iostream>

Engine::Engine( int enable ) {
	m_features =enable;
	initFeatures();
}

Engine::~Engine() {

}
	
void
Engine::enableFeature( Feature f ) {
	m_features |= f;
}

void 
Engine::disableFeature( Feature f ) {
	m_features &= ~(int)f;
	initFeatures();
}

bool
Engine::isEnabledFeature( Feature f ) {
	return (m_features & f);
	initFeatures();
}

void
Engine::draw( Viewport* viewport ) {

	World *world =viewport->world;
	Camera *cam =viewport->camera;
	
	if( !world || !cam )
		return;
		
	Zone *z =world->currentZone();
	
	if( !z )
		return;
		
	preRender( viewport );
	
	// Setup camera matrices
	
	if( cam->aspect() != viewport->aspect ) {
		cam->setAspect( viewport->aspect );
		cam->update();
	}
	
	cam->orbit( 0.01f, 0.01f );
	//cam->move( Camera::BACK, 0.01 );
//	cam->rotate( 0.01f, 0 );
	cam->update();
	
	glm::mat4 matprojection = cam->projectionMatrix();
	glm::mat4 matview = cam->viewMatrix();
	
	// Traverse the object-tree in a DFS manner
	
	std::stack<Object*> obj_stack;
	std::stack<glm::mat4> mat_stack;
	obj_stack.push( z );
	mat_stack.push( modelMat( z ) );
	
	while( !obj_stack.empty() ) {
		
		Object *o = obj_stack.top();
		glm::mat4 matmodel = mat_stack.top();
		obj_stack.pop();
		mat_stack.pop();
			
		// Only actor types contain drawable geometry
		
		if( o->isA( Actor::TYPE ) ) {
			Actor *actor = reinterpret_cast<Actor*>(o);
			Geometry *g =actor->geometry();
			
			GLuint p = g->program();
			glUseProgram( p );
			
			glm::mat4 matmodelview = matview * matmodel;
			
			glUniformMatrix4fv(glGetUniformLocation(p, "matmodelview"), 1, GL_FALSE, glm::value_ptr(matmodelview));
			
			glUniformMatrix4fv(glGetUniformLocation(p, "matprojection"), 1, GL_FALSE, glm::value_ptr(matprojection));
			
			glBindVertexArray( g->vao() );
			glDrawArrays( g->type(), g->first(), g->size() );
		}
			
		Object::Objects objs =o->getChildren();
		
		if( !objs.empty() ) {
			for( Object::Objects::iterator it =objs.begin();it !=objs.end(); it++ ){
				obj_stack.push( (*it) );
				mat_stack.push( matmodel * modelMat(*it)  );
			}
		}
		
	}

}

void
Engine::initFeatures() {

	if( isEnabledFeature( FT_DEPTHBUFFER ) ) {
		glDepthFunc( GL_LEQUAL );
		glEnable( GL_DEPTH_TEST );
		glClearDepth(1.0f);
	
	} else {
		glDisable( GL_DEPTH_TEST );
	}
	
	if( isEnabledFeature( FT_ALPHABLEND ) ) {
		glEnable( GL_BLEND );
		glBlendFunc( GL_ONE, GL_ONE );
	} else {
		glDisable( GL_BLEND );
	}
}

void
Engine::preRender( Viewport* v ) {
	if( isEnabledFeature( FT_DEPTHBUFFER ) ) {
		glClear( GL_DEPTH_BUFFER_BIT );	
	}
	
	glClearColor( v->background.r, v->background.g, v->background.b, v->background.a );
	glClear( GL_COLOR_BUFFER_BIT );
	
}

glm::mat4 
Engine::modelMat( Object* obj ) {

	//glm::mat4 mat = glm::translate( obj->position() );
	
	return obj->transform().matrix();
}