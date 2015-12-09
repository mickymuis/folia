#include "pointsprites.h"
#include "../utils/shaderprogram.h"
#include "../common/shaderlayout.h"


PointSpritesGeometry::PointSpritesGeometry() 
{
	m_program =0;
	m_type =GL_POINTS;
	initBuffers();
}

PointSpritesGeometry::~PointSpritesGeometry(){
	destroyBuffers();
}

void 
PointSpritesGeometry::setProgram( ShaderProgram* p ) {
	m_program =p;
}

void 
PointSpritesGeometry::updateBuffers() {
	glBindVertexArray(m_vao);
	
	glBindBuffer(GL_ARRAY_BUFFER, vbo_points );
	glEnableVertexAttribArray( S_VERTEX_ATTRIB_POSITION );
	glEnableVertexAttribArray( S_VERTEX_ATTRIB_PARAM );
	
	glVertexAttribPointer( 	S_VERTEX_ATTRIB_POSITION,  // vertex attrib array index
													3, 												 // number of elements
													GL_FLOAT, 								 // data type
													GL_FALSE, 								 // normalize?
													sizeof(GLfloat) * 6,			 // stride
													0 												 // offset
												);
	
	glVertexAttribPointer( 	S_VERTEX_ATTRIB_PARAM,  // vertex attrib array index
													3, 												 // number of elements
													GL_FLOAT, 								 // data type
													GL_FALSE, 								 // normalize?
													sizeof(GLfloat) * 6,			 // stride
													(void*)(3*sizeof(GLfloat)) // offset
												);
	glBufferData( GL_ARRAY_BUFFER, m_data.size() * sizeof( Point ), &m_data[0], GL_STATIC_DRAW );
	
	glBindVertexArray( 0 );
}

void PointSpritesGeometry::initBuffers() {
	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);
	
	glGenBuffers(1, &vbo_points);
	
	updateBuffers();
}

void 
PointSpritesGeometry::destroyBuffers() {
	glDeleteVertexArrays( 1, &m_vao );
	glDeleteBuffers( 1, &vbo_points );
}

/* Class PointSprites */

PointSprites::PointSprites( Object* parent ) : Actor ( parent ) 
{}

PointSprites::~PointSprites() 
{}

int 
PointSprites::append( glm::vec3 position, float radius ) {

	PointSpritesGeometry::Point p = { position, glm::vec3( radius, 0.0f, 0.0f ) };
	geom.data().push_back( p );
	
	geom.updateBuffers();
	
	return geom.data().size()-1;
}

void 
PointSprites::remove( int i ) {
	geom.data().erase( geom.data().begin() + i );
	geom.updateBuffers();
}

void 
PointSprites::setPosition( int i, glm::vec3 pos ) {
	geom.data()[i].position =pos;
	geom.updateBuffers();
}

void 
PointSprites::setRadius( int i, float r ) {
	geom.data()[i].params.x =r;
	geom.updateBuffers();
}

int 
PointSprites::count() const {
	return geom.data().size();
}

void 
PointSprites::setProgram( ShaderProgram* p ) {
	geom.setProgram( p );
}

Geometry* 
PointSprites::geometry( int ) {
	return &geom;
}

