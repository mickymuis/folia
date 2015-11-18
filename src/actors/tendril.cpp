#include "tendril.h"
#include "../utils/shaderprogram.h"

/* TendrilGeometry implementation */

static const GLfloat normal_dummy[] {
		0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, -1.0f
};

TendrilGeometry::TendrilGeometry() {
	m_type = GL_TRIANGLES;
	m_size = 3;
	
	ShaderSource src;
	src.addSource( VERTEX_SHADER, "tendril" );
	src.addSource( GEOMETRY_SHADER, "tendril" );
	src.addSource( FRAGMENT_SHADER, "tendril" );
/*	src.addSource( VERTEX_SHADER, "test" );
	src.addSource( FRAGMENT_SHADER, "test" );*/
	
	m_program = src.createProgram();

	initBuffers();
}

TendrilGeometry::~TendrilGeometry() {
	destroyBuffers();
}

void 
TendrilGeometry::setPosition( int index, glm::vec3 v ) {

	if( index >= m_size )
		return;
		
	vertices[index*3] = v.x;
	vertices[index*3+1] = v.y;
	vertices[index*3+2] = v.z;
}

void 
TendrilGeometry::initBuffers() {
	
	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);
	
	glGenBuffers(1, &vbo_position);
	
	glGenBuffers(1, &vbo_normal);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_normal );
	glEnableVertexAttribArray( 1 );
	glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 0, 0 );
	glBufferData( GL_ARRAY_BUFFER, sizeof(normal_dummy), normal_dummy, GL_STATIC_DRAW );
	
	updateBuffers();
	
	
	
}

void
TendrilGeometry::updateBuffers() {
	glBindVertexArray(m_vao);
	
	glBindBuffer(GL_ARRAY_BUFFER, vbo_position );
	glEnableVertexAttribArray( 0);
	glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, 0 );
	glBufferData( GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW );
	
	glBindVertexArray( 0 );
}

void 
TendrilGeometry::destroyBuffers() {
	glDeleteVertexArrays( 1, &m_vao );
	glDeleteBuffers( 1, &vbo_position );
	//glDeleteBuffers( 1, &vbo_normal );
}

/* Tendril implementation */

Tendril::Tendril( Object* parent ) : Actor( parent ) {

	geom.setPosition( 0, glm::vec3( 0,0,0) ); // Base
	geom.setPosition( 1, glm::vec3( -0.5,3,-1) ); // Shoulder
	geom.setPosition( 2, glm::vec3( 2,4,0) ); // Head
	
	geom.updateBuffers();
}

Tendril::~Tendril() {
}

Geometry* 
Tendril::geometry() {
	return &geom;
}
	