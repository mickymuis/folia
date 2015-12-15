/*
 * This file is part of Folia, an experimental mini-engine using OpenGL 3
 * Created by Micky Faas. Freely usable and modifiable for academic purposes.
 */

#define GLM_SWIZZLE
#include "../common/platform.h"
#include "engine.h"
#include "window.h"
#include "zone.h"
#include "world.h"
#include "actor.h"
#include "geometry.h"
#include "camera.h"
#include "object.h"
#include "light.h"
#include "../utils/shaderprogram.h"
#include "../utils/glm/glm.hpp"
#include "../utils/glm/gtc/type_ptr.hpp"
#include "../utils/glm/gtc/matrix_transform.hpp"
#include "../utils/glm/gtx/transform.hpp"
#include "../actors/solidcube.h"
#include <stack>
#include <iostream>
#include <stdio.h>

GLfloat ScreenQuad::data[20] = {
		// Positions        // Texture Coords
		-1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
		1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
};

ScreenQuad::ScreenQuad() : vbo_position( 0 ) {

	m_type = GL_TRIANGLE_STRIP;
	m_size = 4;
	
}

bool 
ScreenQuad::initialize() {
	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);
	
	glGenBuffers(1, &vbo_position);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_position );
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glBufferData( GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW );
	
	glBindVertexArray( 0 );
	
	ShaderSource src;
	src.addSource( VERTEX_SHADER, "ss_light" );
	src.addSource( FRAGMENT_SHADER, "ss_light" );
	m_program = src.createProgram();
	
	if( !m_program ) {
		std::cerr << src.errorMessage() << std::endl;
		return false;
	}
	
	glUseProgram( m_program->programHandle() );
	glUniform1i(glGetUniformLocation(m_program->programHandle(), "g_depth"), 0);
	glUniform1i(glGetUniformLocation(m_program->programHandle(), "g_diffusespec"), 1);
  glUniform1i(glGetUniformLocation(m_program->programHandle(), "g_normal"), 2);
  glUniform1i(glGetUniformLocation(m_program->programHandle(), "g_position"), 3);
  glUseProgram( 0 );
	
	return true;
}

void 
ScreenQuad::destroy() {
	glDeleteVertexArrays( 1, &m_vao );
	glDeleteBuffers( 1, &vbo_position );
}

/* Engine implementation */

Engine::Engine( int enable ) {
	m_features =enable;
	
	initialize();
	//initFeatures();
}

Engine::~Engine() {
	m_gbuffer.destroy();
	m_screen.destroy();
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

	if( m_hasError )
		return;
		
	World *world =viewport->world;
	Camera *cam =viewport->camera;
	
	if( !world || !cam )
		return;
		
	Zone *z =world->currentZone();
	
	if( !z )
		return;
		
	//preRender( viewport );
	
	// Setup camera matrices
	
	if( cam->aspect() != viewport->aspect ) {
		cam->setAspect( viewport->aspect );
		cam->update();
	}
	
	glm::mat4 matprojection = cam->projectionMatrix();
	glm::mat4 matview = cam->viewMatrix();
	
	// Rebuild the render queue
	
	m_queue.setRootObject( z );
	m_queue.rebuild( );
	
	// (Re)initialize the g-buffer
	
	if( m_gbuffer.width() != viewport->width || m_gbuffer.height() != viewport->height )
		m_gbuffer.initialize( *viewport, true, 2 );
	
	
	// Geometry pass
	
	m_gbuffer.bindForWriting();	
	
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glDisable(GL_BLEND);
	//glEnable(GL_MULTISAMPLE);
	
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
//	glClearDepth( 0.0f );
	//glClearColor( viewport->background.r, viewport->background.g, viewport->background.b, viewport->background.a );
	glClear( GL_COLOR_BUFFER_BIT );
	glClear( GL_DEPTH_BUFFER_BIT );
	
	renderQueue( m_queue, matview, matprojection, DEFERRABLE );
	
	// Deferred lighting pass
	
	m_gbuffer.bindForReading();
	
	glDisable(GL_DEPTH_TEST);
	glDepthMask(GL_FALSE);
	glEnable(GL_BLEND);
	//glDisable(GL_MULTISAMPLE);
	glBlendEquation(GL_FUNC_ADD);
	glBlendFunc(GL_ONE, GL_ONE);
		
	GLuint p =m_screen.program()->programHandle();
	glUseProgram( p );
	
	glBindVertexArray( m_screen.vao() );
	
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear( GL_COLOR_BUFFER_BIT /*| GL_DEPTH_BUFFER_BIT*/ );
	
	glm::mat4x4 matwindowtoview= glm::inverse( matprojection );
	
	glm::vec3 view_position = glm::vec3(matview * glm::vec4(cam->position(), 1 ) );
//	glm::vec3 view_position = cam->position();
	
	glUniform3fv( glGetUniformLocation(p, "view_position"), 1, glm::value_ptr( view_position ) );
	glUniformMatrix4fv(glGetUniformLocation(p, "mat_windowtoview"), 1, GL_FALSE, glm::value_ptr(matwindowtoview));
	glUniformMatrix4fv(glGetUniformLocation(p, "mat_view"), 1, GL_FALSE, glm::value_ptr( matview ));
	
	const std::vector<Light*> &lights =z->lights();
	for( int i =0; i < lights.size(); i++ ) {
	
		Light* l =lights[i];
		if( !l->isVisible() )
			continue;
			
		glm::vec3 light_position = glm::vec3(matview * glm::vec4(l->transform().position(), 1 ));
//		glm::vec3 light_position =l->transform().position();

		glm::vec3 light_direction;
		if( l->type() == Light::LIGHT_POINT )
			light_direction =glm::vec3(0);
		else
			light_direction =glm::vec3(matview * glm::vec4(l->direction(), 0 ));
			
		glm::vec3 light_attenuation = glm::vec3( 1.0f,
			l->linearAttenuation(),
			l->quadraticAttenuation() );
		
		glUniform3fv( glGetUniformLocation(p, "light_position"), 1, glm::value_ptr( light_position ) );
		glUniform3fv( glGetUniformLocation(p, "light_direction"), 1, glm::value_ptr( light_direction ) );
		glUniform3fv( glGetUniformLocation(p, "light_intensity"), 1, glm::value_ptr( l->intensity()) );
		glUniform3fv( glGetUniformLocation(p, "light_attenuation"), 1, glm::value_ptr( light_attenuation ) );
		
		render( &m_screen, 0 );
		
	}
	
	// Forward render pass
	
	//glEnable( GL_MULTISAMPLE );
	
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0); // Write to default framebuffer
	glBlitFramebuffer(
		0, 0, m_gbuffer.width(), m_gbuffer.height(), 0, 0, m_gbuffer.width(), m_gbuffer.height(), GL_DEPTH_BUFFER_BIT, GL_NEAREST
	);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	
	glEnable(GL_DEPTH_TEST);
	//glDepthMask(GL_TRUE);
	
	glEnable(GL_BLEND);
	glBlendEquation(GL_FUNC_ADD);
	glBlendFunc(GL_ONE, GL_ONE);

	renderQueue( m_queue, matview, matprojection, FORWARD_ONLY );

}

void
Engine::initFeatures() {

	if( isEnabledFeature( FT_DEPTHBUFFER ) ) {
		glDepthFunc( GL_LEQUAL );
		glEnable( GL_DEPTH_TEST );
		//glClearDepth(1.0f);
	
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
Engine::initialize() {
	m_hasError =false;
	
	printf( "Engine::initialize()\n" );
	
	if( !m_screen.initialize() ) {
		m_hasError =true;
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

void 
Engine::renderQueue( const RenderQueue& queue, glm::mat4 matview, glm::mat4x4 matprojection, int filters ) {
	
	const RenderQueue::queue_t &q = queue.queue(); 
	
	RenderQueue::queue_t::const_iterator it;
	for( it =q.begin(); it != q.end(); it++ ) {
		const RenderQueueTuple& tuple =(*it);
		
		Geometry *g =tuple.geometry;		
		if( !filter( g, filters ) )
			continue;
			
		for( int i =0; i < g->bufferCount(); i++ ) {
			
			ShaderProgram *program = g->program(i);
		
			if( program ) {
				GLuint p =program->programHandle();
				glUseProgram( p );
				
				glm::mat4x4 matmodelview =matview * tuple.matmodel;
	
				glUniformMatrix4fv(glGetUniformLocation(p, "mat_model"), 1, GL_FALSE, glm::value_ptr( tuple.matmodel ));
				glUniformMatrix4fv(glGetUniformLocation(p, "mat_view"), 1, GL_FALSE, glm::value_ptr( matview ));
				glUniformMatrix4fv(glGetUniformLocation(p, "mat_projection"), 1, GL_FALSE, glm::value_ptr(matprojection));
			}
		
			render( g, i );
		}
		
	}
}

void 
Engine::render( Geometry* g, int i ) {

	glBindVertexArray( g->vao(i) );
	g->preRender( i );
	glDrawArrays( g->type(i), g->first(i), g->size(i) );
}

bool 
Engine::filter( Geometry *g, int filters ) {
	
	if( !g )
		return false;
	
	if( filters == ALL )
		return true;
		
	if( filters & DEFERRABLE && g->renderDeferred() )
		return true;
	
	if( filters & FORWARD_ONLY && !g->renderDeferred() )
		return true;
		
	return false;
}
