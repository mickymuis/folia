#include "tendrilfield.h"
#include "../utils/shaderprogram.h"
#include "../common/shaderlayout.h"

/* Tendril */

void 
Tendril::setAttrib( Attrib a , glm::vec3 v ) {
	data[(int)a * 3] = v.x;
	data[(int)a * 3 + 1] = v.y;
	data[(int)a * 3 + 2] = v.z;
}

void 
Tendril::setAttrib( AttribElement e, float f ) {
	data[(int)e] = f;
}

glm::vec3 
Tendril::attribVector( Attrib a ) const {
	return glm::vec3 (
		data[(int)a * 3],
		data[(int)a * 3 + 1],
		data[(int)a * 3 + 2] 
	);	
}

/* TendrilGeometry implementation */

TendrilGeometry::TendrilGeometry() {
	m_type = GL_TRIANGLES;
	
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

size_t 
TendrilGeometry::append( int num ) {
	data.reserve( num );
	
	size_t index =data.size();
	
	for( int i=0; i < num; i++ )
		data.push_back(Tendril());
		
	return index;
}

size_t 
TendrilGeometry::append( const Tendril& t ) {
	data.push_back( t );
	return data.size()-1;
}
		
void 
TendrilGeometry::setDataAt( int index, const Tendril& t ) {
	data[index] =t;
}

Tendril& 
TendrilGeometry::dataAt( int index ) {
	return data[index];
}
	
size_t
TendrilGeometry::tendrilCount() const {
	return data.size();
}

void 
TendrilGeometry::initBuffers() {
	
	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);
	
	glGenBuffers(1, &vbo_position);
	//glGenBuffers(1, &vbo_param);
	
	updateBuffers();
	
	
	
}

void
TendrilGeometry::updateBuffers() {
	glBindVertexArray(m_vao);
	
	glBindBuffer(GL_ARRAY_BUFFER, vbo_position );
	glEnableVertexAttribArray( S_VERTEX_ATTRIB_POSITION );
	glEnableVertexAttribArray( S_VERTEX_ATTRIB_PARAM );
	
	glVertexAttribPointer( 	S_VERTEX_ATTRIB_POSITION,  // vertex attrib array index
													3, 												 // number of elements
													GL_FLOAT, 								 // data type
													GL_FALSE, 								 // normalize?
													sizeof(GLfloat) * 6,			 // stride
													0 												 // offset
												);
	//glBufferData( GL_ARRAY_BUFFER, data.size() * sizeof( Tendril ) / 2, &data, GL_STATIC_DRAW );
	
	//glBindBuffer(GL_ARRAY_BUFFER, vbo_param );
	
	glVertexAttribPointer( 	S_VERTEX_ATTRIB_PARAM,  // vertex attrib array index
													3, 												 // number of elements
													GL_FLOAT, 								 // data type
													GL_FALSE, 								 // normalize?
													sizeof(GLfloat) * 6,			 // stride
													(void*)(3*sizeof(GLfloat)) // offset
												);
	glBufferData( GL_ARRAY_BUFFER, data.size() * sizeof( Tendril ), &data[0], GL_STATIC_DRAW );
	
	glBindVertexArray( 0 );
}

void 
TendrilGeometry::destroyBuffers() {
	glDeleteVertexArrays( 1, &m_vao );
	glDeleteBuffers( 1, &vbo_position );
	//glDeleteBuffers( 1, &vbo_param );
}

/* Tendril implementation */

TendrilField::TendrilField( Object* parent ) : Actor( parent ) {

	Tendril t;
	
	t.setAttrib( Tendril::BASE, glm::vec3( 0,0,0) ); // Base
	t.setAttrib( Tendril::SHOULDER, glm::vec3( -1,3,0) ); // Shoulder
	t.setAttrib( Tendril::HEAD, glm::vec3( 1,4,0.0) ); // Head
	t.setAttrib( Tendril::THICKNESS, 0.05 ); // Head
	
	geom.append( t );
	//geom.append( t );

	
	geom.updateBuffers();
}

TendrilField::~TendrilField() {
}

Geometry* 
TendrilField::geometry() {
	return &geom;
}
	