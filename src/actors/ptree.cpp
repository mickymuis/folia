#include "ptree.h"
#include "../utils/shaderprogram.h"
#include "../common/shaderlayout.h"
#include "../utils/glm/gtc/random.hpp"
#include "../utils/glm/gtx/rotate_vector.hpp"


GLfloat BRANCH_SECTION_DEFAULT[] {

	0.0, -1.0, 0.0, 	// prev
	0.0, 0.0, 0.0,  // base: radius, rough, seed
	0.0, 0.0, 0.0, 	// base
	0.0, 0.0, 0.0,  // head: radius, rough, seed
	0.0, 1.0, 0.0,  // head
	0.0, 0.0, 0.0,  // curvature
	0.0, 0.0, 0.0,  // next
	4.0, 4.0, 0.0	// params: sections, steps, reserved
};

/* Tendril */

void 
PBranchSection::setAttrib( Attrib a , glm::vec3 v ) {
	data[(int)a * 3] = v.x;
	data[(int)a * 3 + 1] = v.y;
	data[(int)a * 3 + 2] = v.z;
}

void 
PBranchSection::setAttrib( AttribElement e, float f ) {
	data[(int)e] = f;
}

glm::vec3 
PBranchSection::attribVector( Attrib a ) const {
	return glm::vec3 (
		data[(int)a * 3],
		data[(int)a * 3 + 1],
		data[(int)a * 3 + 2] 
	);	
}

float
PBranchSection::length() const {
	return glm::length( attribVector( HEAD ) - attribVector( BASE ) );
}

PBranchSection::PBranchSection() {
	memcpy( data, BRANCH_SECTION_DEFAULT, NUM_FLOATS * sizeof( GLfloat ) );
}

/* TendrilGeometry implementation */

PTreeGeometry::PTreeGeometry() {
	m_type = GL_LINES_ADJACENCY;
	
	ShaderSource src;
	src.addSource( VERTEX_SHADER, "ptree_branch" );
	src.addSource( GEOMETRY_SHADER, "ptree_branch" );
	src.addSource( FRAGMENT_SHADER, "ptree_branch" );
	
	m_program = src.createProgram();

	initBuffers();
}

PTreeGeometry::~PTreeGeometry() {
	destroyBuffers();
}

size_t 
PTreeGeometry::append( int num ) {
	data.reserve( num );
	
	size_t index =data.size();
	
	for( int i=0; i < num; i++ )
		data.push_back(PBranchSection());
		
	return index;
}

size_t 
PTreeGeometry::append( const PBranchSection& t ) {
	data.push_back( t );
	return data.size()-1;
}
		
void 
PTreeGeometry::setDataAt( int index, const PBranchSection& t ) {
	data[index] =t;
}

PBranchSection& 
PTreeGeometry::dataAt( int index ) {
	return data[index];
}
	
size_t
PTreeGeometry::branchCount() const {
	return data.size();
}

void 
PTreeGeometry::initBuffers() {
	
	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);
	
	glGenBuffers(1, &vbo_position);
	
	updateBuffers();
}

void
PTreeGeometry::updateBuffers() {
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
	
	glVertexAttribPointer( 	S_VERTEX_ATTRIB_PARAM,  // vertex attrib array index
													3, 												 // number of elements
													GL_FLOAT, 								 // data type
													GL_FALSE, 								 // normalize?
													sizeof(GLfloat) * 6,			 // stride
													(void*)(3*sizeof(GLfloat)) // offset
												);
	glBufferData( GL_ARRAY_BUFFER, data.size() * sizeof( PBranchSection ), &data[0], GL_STATIC_DRAW );
	
	glBindVertexArray( 0 );
}

void 
PTreeGeometry::destroyBuffers() {
	glDeleteVertexArrays( 1, &m_vao );
	glDeleteBuffers( 1, &vbo_position );
}

/* PTree implementation */

PTree::PTree( Object* parent ) : Actor( parent ) {

	constraints[0].max_extensions = 10;	
	constraints[0].sections =20;		
	constraints[0].steps =2;
	constraints[0].max_radius =1.0;
	constraints[0].min_radius =0.5;
	constraints[0].max_length =2.0;
	constraints[0].min_length =1.0;
	constraints[0].max_total_length =15.0;
	constraints[0].max_curvature =0.5;				
	constraints[0].max_rough =1.0;
	constraints[0].max_growth_length =0.8;
	constraints[0].max_growth_radius =0.04;
	constraints[0].min_branch_distance =3;
	
	
	constraints[1].max_extensions = 8;	
	constraints[1].sections =10;		
	constraints[1].steps =5;
	constraints[1].max_radius =0.3;
	constraints[1].min_radius =0.1;
	constraints[1].max_length =0.8;
	constraints[1].min_length =0.4;
	constraints[1].max_total_length =7.0;
	constraints[1].max_curvature =0.1;				
	constraints[1].max_rough =0.5;
	constraints[1].max_growth_length =0.4;
	constraints[1].max_growth_radius =0.02;
	constraints[1].min_branch_distance =2;
	
	constraints[2].max_extensions = 4;	
	constraints[2].sections =6;		
	constraints[2].steps =3;
	constraints[2].max_radius =0.2;
	constraints[2].min_radius =0.05;
	constraints[2].max_length =0.4;
	constraints[2].min_length =0.1;
	constraints[2].max_total_length =3.0;
	constraints[2].max_curvature =0.05;				
	constraints[2].max_rough =0.2;
	constraints[2].max_growth_length =0.4;
	constraints[2].max_growth_radius =0.01;
	constraints[2].min_branch_distance =1;

	root =createNode(0);
	root->up = glm::vec3(0,1,0);
	root->branches =0;
	
	geom.updateBuffers();
}

PTree::~PTree() {
}

Geometry* 
PTree::geometry() {
	return &geom;
}

void 
PTree::update( float deltatime ) {

	growRecursive( 0, root, 0, deltatime );
	geom.updateBuffers();
	Actor::update( deltatime );
}

PTree::Node::Node() 
	: index( 0 ),
		level( 0 ),
		extension( 0 ),
		branch( 0 ) { }
		
void 
PTree::growRecursive( Node* parent, Node* n, int extension_count, float deltatime ) {
	
	if( !n )
		return;
		
	const Constraint& c= constraints[n->level];
	
//	printf( "%f\n\n", deltatime * glm::linearRand( 0.0f, c.max_growth_radius ) );
	
	if( parent ) {
		//if( extension_count )
			data(n).setAttrib( PBranchSection::BASE_RADIUS, 
				data(parent).data[PBranchSection::HEAD_RADIUS] * (!extension_count ? 0.8 : 1.0) );
		data(n).setAttrib( PBranchSection::PREV, 
			data(parent).attribVector(PBranchSection::BASE) );
		data(n).setAttrib( PBranchSection::BASE, 
			data(parent).attribVector( PBranchSection::HEAD ) );
		data(n).setAttrib( PBranchSection::HEAD, 
			data(n).attribVector( PBranchSection::HEAD ) + parent->last_growth );
	}
	else {
		float base_radius =data(n).data[PBranchSection::BASE_RADIUS];
		if( base_radius < n->max_radius ) {
			base_radius += glm::linearRand( 0.0f, c.max_growth_radius ) * deltatime;
			data(n).setAttrib( PBranchSection::BASE_RADIUS, base_radius );
		}
	}
	
	glm::vec3 up = n->up;//data(n).attribVector( PBranchSection::HEAD ) - data(n).attribVector( PBranchSection::BASE );
	n->age += deltatime;
	

	if( data(n).length() < n->max_length ) {
		n->last_growth = glm::normalize( up ) * glm::linearRand( 0.0f, c.max_growth_length ) * deltatime;
		glm::vec3 head =data(n).attribVector( PBranchSection::HEAD );
		head += n->last_growth;
		data(n).setAttrib( PBranchSection::HEAD, head );
	}
	else
		n->last_growth =glm::vec3(0);
		
	if( data(n).length() > n->max_length * 0.25
	&& !n->extension 
	&& extension_count < c.max_extensions ) {
		n->extension =createNode( n->level );
		
		if( extension_count < c.min_branch_distance ) 
			n->extension->branches =0;
		
		glm::vec3 axis = glm::sphericalRand( 1.0 );
		float angle = glm::linearRand( 0.001, 0.05 );
		
		glm::vec3 offset = glm::rotate( up, angle, axis );
		
		data(n->extension).setAttrib( PBranchSection::HEAD, data(n).attribVector( PBranchSection::HEAD ) + glm::vec3(0.05) );
		n->extension->max_radius *= sqrt( 1.0 / (extension_count+2));
		n->extension->up = offset;
	}

		
	if( n->extension ) {
		float head_radius =data(n).data[PBranchSection::HEAD_RADIUS];
		if( head_radius < n->max_radius ) {
			head_radius += glm::linearRand( 0.0f, c.max_growth_radius ) * deltatime;
			data(n).setAttrib( PBranchSection::HEAD_RADIUS, head_radius );
		}
	}
	
	if( data(n).data[ PBranchSection::HEAD_RADIUS ] > n->max_radius * 0.15
		&& !n->branch 
		&& n->level != MAX_LEVEL -1
		&& n->branches ) {
		
		printf( "Branch!\n" );
		
		n->branch =createNode( n->level + 1 );
		
		if( extension_count < c.min_branch_distance ) 
			n->branch->branches =0;
		
		glm::vec3 axis = glm::cross( data(n).attribVector( PBranchSection::HEAD ), data(n).attribVector( PBranchSection::BASE ) );
		axis = glm::rotate( axis, (float)glm::linearRand( 0.0, 2* M_PI ), up );
		float angle = glm::linearRand( 0.5, 1.5 );
		
		glm::vec3 offset = glm::rotate( up, angle, axis );
		
		data(n->branch).setAttrib( PBranchSection::HEAD, data(n).attribVector( PBranchSection::HEAD ) + glm::vec3(0.05) );
//		n->branch->max_radius *= powf( 1.0 / (extension_count+2), 2);
		n->branch->up = offset;
		
	}
	
	data(n).setAttrib( PBranchSection::CURVE, n->curve * data(n).length() / n->max_length );
	
	growRecursive( n, n->extension, extension_count+1, deltatime );
	growRecursive( n, n->branch, 0, deltatime );
	
}

PTree::Node*
PTree::createNode( int level ) {
	PTree::Node *n =new PTree::Node();
	int i =geom.append(1);
	n->index =i;
	n->age =0;
	n->level =level;
	n->max_length = glm::linearRand( constraints[level].min_length, constraints[level].max_length );
	n->max_radius = glm::linearRand( constraints[level].min_radius, constraints[level].max_radius );
	n->curve = glm::ballRand( constraints[level].max_curvature );
	n->branches = glm::linearRand( 1.0, 2.0 );
	
	data(n).setAttrib( PBranchSection::SECTIONS, constraints[level].sections );
	data(n).setAttrib( PBranchSection::STEPS, constraints[level].steps );
	
	printf( "PTree: new level %d node\n", level );
	
	return n;
}

PBranchSection&
PTree::data( Node* n ) {
	return geom.dataAt( n->index );
}
