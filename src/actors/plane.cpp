/*
 * This file is part of Folia, an experimental mini-engine using OpenGL 3
 * Created by Micky Faas. Freely usable and modifiable for academic purposes.
 */

#include "plane.h"
#include "../utils/shaderprogram.h"
#include "../common/shaderlayout.h"

/* Grey unit plane */
GLfloat PLANE_DEFAULT[] = {
	/* position BOTTOM_LEFT */ 
	-0.5, -0.5, 0.0, 
	/* normal */
	0.0, 0.0, 1.0,
	/* color */ 
	0.5, 0.7, 0.5, 1.0,
	/* texcoord */
	0.0, 0.0,
	/* tangent */
	0.0, 1.0, 0.0,
	
	/* position TOP_LEFT */ 
	-0.5, 0.5, 0.0, 
	/* normal */
	0.0, 0.0, 1.0,
	/* color */ 
	0.0, 0.0, 0.0, 1.0,
	/* texcoord */
	0.0, 1.0,
	/* tangent */
	0.0, 1.0, 0.0,
	
	/* position BOTTOM_RIGHT */ 
	0.5, -0.5, 0.0, 
	/* normal */
	0.0, 0.0, 1.0,
	/* color */ 
	0.5, 0.7, 0.5, 1.0,
	/* texcoord */
	0.1, 0.0,
	/* tangent */
	0.0, 1.0, 0.0,
	
	/* position  TOP_RIGHT */ 
	0.5, 0.5, 0.0, 
	/* normal */
	0.0, 0.0, 1.0,
	/* color */ 
	0.0, 0.0, 0.0, 1.0,
	/* texcoord */
	1.0, 1.0,
	/* tangent */
	0.0, 1.0, 0.0
};

PlaneGeometry::PlaneGeometry() 
{
	m_program =0;
	m_type =GL_TRIANGLE_STRIP;
	memcpy( m_data, PLANE_DEFAULT, sizeof( PLANE_DEFAULT ) );
	
	ShaderSource src;
	src.addSource( VERTEX_SHADER, "solidgeometry" );
	src.addSource( FRAGMENT_SHADER, "solidgeometry" );
	m_program = src.createProgram();
	
	initBuffers();
}

PlaneGeometry::~PlaneGeometry(){
	destroyBuffers();
}

void 
PlaneGeometry::setProgram( ShaderProgram* p ) {
	m_program =p;
}

void 
PlaneGeometry::updateBuffers() {
	glBindVertexArray(m_vao);
	
	glBindBuffer(GL_ARRAY_BUFFER, vbo_points );

	glEnableVertexAttribArray( S_VERTEX_ATTRIB_POSITION );
	glEnableVertexAttribArray( S_VERTEX_ATTRIB_NORMAL );
	glEnableVertexAttribArray( S_VERTEX_ATTRIB_COLOR );
	glEnableVertexAttribArray( S_VERTEX_ATTRIB_TEXCOORDS );
	glEnableVertexAttribArray( S_VERTEX_ATTRIB_TANGENT );
	
	glVertexAttribPointer( 	S_VERTEX_ATTRIB_POSITION,  // vertex attrib array index
													3, 												 // number of elements
													GL_FLOAT, 								 // data type
													GL_FALSE, 								 // normalize?
													sizeof(GLfloat) * 15,			 // stride
													0 												 // offset
												);
	
	glVertexAttribPointer( 	S_VERTEX_ATTRIB_NORMAL,  // vertex attrib array index
													3, 												 // number of elements
													GL_FLOAT, 								 // data type
													GL_FALSE, 								 // normalize?
													sizeof(GLfloat) * 15,			 // stride
													(void*)(3*sizeof(GLfloat)) // offset
												);
												
	glVertexAttribPointer( 	S_VERTEX_ATTRIB_COLOR,  // vertex attrib array index
													3, 												 // number of elements
													GL_FLOAT, 								 // data type
													GL_FALSE, 								 // normalize?
													sizeof(GLfloat) * 15,			 // stride
													(void*)(6*sizeof(GLfloat)) // offset
												);
	
	glVertexAttribPointer( 	S_VERTEX_ATTRIB_TEXCOORDS,  // vertex attrib array index
													3, 												 // number of elements
													GL_FLOAT, 								 // data type
													GL_FALSE, 								 // normalize?
													sizeof(GLfloat) * 15,			 // stride
													(void*)(10*sizeof(GLfloat)) // offset
												);
												
	glVertexAttribPointer( 	S_VERTEX_ATTRIB_TANGENT,  // vertex attrib array index
													3, 												 // number of elements
													GL_FLOAT, 								 // data type
													GL_FALSE, 								 // normalize?
													sizeof(GLfloat) * 15,			 // stride
													(void*)(12*sizeof(GLfloat)) // offset
												);
												
	glBufferData( GL_ARRAY_BUFFER, 4 * sizeof( Point ), &m_data[0], GL_STATIC_DRAW );
	
	glBindVertexArray( 0 );
}

void PlaneGeometry::initBuffers() {
	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);
	
	glGenBuffers(1, &vbo_points );
	
	updateBuffers();
}

void 
PlaneGeometry::destroyBuffers() {
	glDeleteVertexArrays( 1, &m_vao );
	glDeleteBuffers( 1, &vbo_points );
}

/* Class Plane */

Plane::Plane( Object* parent ) : Actor ( parent ) 
{}

Plane::~Plane() 
{}

void 
Plane::setProgram( ShaderProgram* p ) {
	geom.setProgram( p );
}

Geometry* 
Plane::geometry( int ) {
	return &geom;
}

