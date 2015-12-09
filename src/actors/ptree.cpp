#include "ptree.h"
#include "../utils/shaderprogram.h"
#include "../utils/sdltextureloader.h"
#include "../common/shaderlayout.h"
#include "../utils/glm/gtc/random.hpp"
#include "../utils/glm/gtx/rotate_vector.hpp"
#include "../utils/glm/gtc/type_ptr.hpp"

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

GLfloat LEAF_DEFAULT[] {

	0.0, 0.0, 0.0, 	// base
	2.5, 1.0, 0.0,  // width, concavity, seed
	0.0, 5.0, 0.0,  // head
	5.0, 0.5, 0.0,  // num_leafs, roundness, reserved
	0.0, 0.0, 1.0,  // up
	0.0, 0.0, 0.0,  // reserved
	
};

/* PBranchSection */

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

/* PLeaf implementation */

void 
PLeaf::setAttrib( Attrib a , glm::vec3 v ) {
	data[(int)a * 3] = v.x;
	data[(int)a * 3 + 1] = v.y;
	data[(int)a * 3 + 2] = v.z;
}

void 
PLeaf::setAttrib( AttribElement e, float f ) {
	data[(int)e] = f;
}

glm::vec3 
PLeaf::attribVector( Attrib a ) const {
	return glm::vec3 (
		data[(int)a * 3],
		data[(int)a * 3 + 1],
		data[(int)a * 3 + 2] 
	);	
}

float
PLeaf::length() const {
	return glm::length( attribVector( TIP ) - attribVector( BASE ) );
}

PLeaf::PLeaf() {
	memcpy( data, LEAF_DEFAULT, NUM_FLOATS * sizeof( GLfloat ) );
}

/* PTreeGeometry implementation */

PTreeGeometry::PTreeGeometry() {

	/* Shader for branches */
	ShaderSource src1;
	src1.addSource( VERTEX_SHADER, "ptree" );
	//src1.addSource( GEOMETRY_SHADER, "ptree_wire" );
	src1.addSource( GEOMETRY_SHADER, "ptree_branch" );
	src1.addSource( FRAGMENT_SHADER, "ptree_branch" );
	m_program = m_branchProgram = src1.createProgram();
	
	/* Shader for leafs */
	ShaderSource src2;
	src2.addSource( VERTEX_SHADER, "ptree" );
	src2.addSource( GEOMETRY_SHADER, "ptree_leaf" );
	src2.addSource( FRAGMENT_SHADER, "ptree_leaf" );
	m_leafProgram = src2.createProgram();
	
	/* Shader for blossoms */
	ShaderSource src3;
	src3.addSource( VERTEX_SHADER, "ptree" );
	src3.addSource( GEOMETRY_SHADER, "ptree_blossom" );
	src3.addSource( FRAGMENT_SHADER, "ptree_blossom" );
	m_blossomProgram = src3.createProgram();
	
	/* Shader for branches */
	ShaderSource src4;
	src4.addSource( VERTEX_SHADER, "ptree" );
	src4.addSource( GEOMETRY_SHADER, "ptree_wire" );
	src4.addSource( FRAGMENT_SHADER, "ptree_branch" );
	m_wireProgram = src4.createProgram();
	
	wind_1 = wind_2 = glm::vec3(0);

	setLeafMode( LEAF );
	initBuffers();
}

PTreeGeometry::~PTreeGeometry() {
	destroyBuffers();
}

size_t 
PTreeGeometry::append( const PBranchSection& t ) {
	branches.push_back( t );
	return branches.size()-1;
}
		
PBranchSection& 
PTreeGeometry::branchAt( int index ) {
	return branches[index];
}
	
size_t
PTreeGeometry::branchCount() const {
	return branches.size();
}

size_t 
PTreeGeometry::append( const PLeaf& t ) {
	leafs.push_back( t );
	return leafs.size()-1;
}
		
PLeaf& 
PTreeGeometry::leafAt( int index ) {
	return leafs[index];
}
	
size_t
PTreeGeometry::leafCount() const {
	return leafs.size();
}

void 
PTreeGeometry::clearLeafs() {
	leafs.clear();
}

void 
PTreeGeometry::clearBranches() {
	branches.clear();
}

void 
PTreeGeometry::initBuffers() {
	
	glGenVertexArrays(1, &m_vao);
	glGenVertexArrays(1, &m_vao_leafs);
	
	glBindVertexArray(m_vao);
	glGenBuffers(1, &vbo_branches);
	
	tex_diffuse1 =loadTextureBMP( "stem1.bmp", true );
	tex_diffuse2 =loadTextureBMP( "bark_pine1.bmp", true );
	tex_normal =loadTextureBMP( "bark_pine1_normal.bmp", true );
	
	if( m_program ) {
		glUseProgram( m_program->programHandle() );
		glUniform1i(glGetUniformLocation(m_program->programHandle(), "tex_diffuse1"), 0);
		glUniform1i(glGetUniformLocation(m_program->programHandle(), "tex_diffuse2"), 1);
		glUniform1i(glGetUniformLocation(m_program->programHandle(), "tex_normal"), 2);
	}
	
	glBindVertexArray(m_vao_leafs);
	glGenBuffers(1, &vbo_leafs);
	
	updateBuffers();
}

void
PTreeGeometry::updateBuffers() {
	glBindVertexArray(m_vao);
	
	if( m_program ) {
		glUseProgram( m_program->programHandle() );
		glUniform3fv( glGetUniformLocation(m_program->programHandle(), "wind_1"), 1, glm::value_ptr(wind_1) );
		glUniform3fv( glGetUniformLocation(m_program->programHandle(), "wind_2"), 1, glm::value_ptr(wind_2) );
	}
	
	/* Branches buffer */
	glBindBuffer(GL_ARRAY_BUFFER, vbo_branches );
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
	glBufferData( GL_ARRAY_BUFFER, branches.size() * sizeof( PBranchSection ), &branches[0], GL_STATIC_DRAW );
	
	//printf( "Branch buffer size: %d sections, %lu vertices, %lu bytes.\n", branches.size(), branches.size()*8, branches.size() * sizeof(PBranchSection));
	
	/* Leafs buffer */
	if( program(1) ) {
		glUseProgram( program(1)->programHandle() );
		glUniform3fv( glGetUniformLocation( program(1)->programHandle(), "wind_1"), 1, glm::value_ptr(wind_1) );
		glUniform3fv( glGetUniformLocation( program(1)->programHandle(), "wind_2"), 1, glm::value_ptr(wind_2) );
	}
	
	
	glBindVertexArray(m_vao_leafs);
	
	glBindBuffer(GL_ARRAY_BUFFER, vbo_leafs );
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
	glBufferData( GL_ARRAY_BUFFER, leafs.size() * sizeof( PLeaf ), &leafs[0], GL_STATIC_DRAW );
	
	glBindVertexArray( 0 );
}

void
PTreeGeometry::preRender( int n ) {
	if( n == 0 ) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, tex_diffuse1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, tex_diffuse2);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, tex_normal);
	}
};

void 
PTreeGeometry::destroyBuffers() {
	glDeleteVertexArrays( 1, &m_vao );
	glDeleteBuffers( 1, &vbo_branches );
	glDeleteBuffers( 1, &vbo_leafs );
}

void 
PTreeGeometry::setWind( glm::vec3 w1, glm::vec3 w2 ) {
	wind_1 =w1;
	wind_2 =w2;
}

void 
PTreeGeometry::setLeafMode( LeafMode m ) {
	leaf_mode =m;
}

void 
PTreeGeometry::setWireMode( bool w ) {
	if( w )
		m_program =m_wireProgram;
	else
		m_program =m_branchProgram;
}

GLuint 
PTreeGeometry::vao( int n  ) const {
	if( n == 1)
		return m_vao_leafs;
	return m_vao;
}

ShaderProgram* 
PTreeGeometry::program( int n  ) const {
	if( n == 1 ) {
		switch( leaf_mode ) {
			case LEAF:
				return m_leafProgram;
				break;	
			case BLOSSOM:
				return m_blossomProgram;
				break;
		}
	}
	return m_program;
}

GLenum 
PTreeGeometry::type( int n ) const {
	if( n == 1 )
		return GL_TRIANGLES;
	return GL_LINES_ADJACENCY;
}

GLint 
PTreeGeometry::first( int n ) const {
	return 0;
}

GLsizei 
PTreeGeometry::size( int n ) const {
	if( n == 1 )
		return leafCount() * 3;
	return branchCount() * 4;
}

/* PTree implementation */

PTree::PTree( Object* parent ) : Actor( parent ) {
	
	constraints[0].max_extensions = 12;	
	constraints[0].min_extensions = 11;	
	constraints[0].sections =12; // 16		
	constraints[0].steps =3;
	constraints[0].max_radius =1.0;
	constraints[0].min_radius =0.5;
	constraints[0].max_length =1.8;
	constraints[0].min_length =1.0;
	constraints[0].max_total_length =15.0;
	constraints[0].max_curvature =0.5;				
	constraints[0].max_rough =0.7;
	constraints[0].max_growth_length =0.8;
	constraints[0].max_growth_radius =0.04;
	constraints[0].min_branch_distance =5;
	
	
	constraints[1].max_extensions = 8;
	constraints[1].min_extensions = 5;	
	constraints[1].sections =8;	//
	constraints[1].steps =4;
	constraints[1].max_radius =0.3;
	constraints[1].min_radius =0.1;
	constraints[1].max_length =1.0;
	constraints[1].min_length =0.5;
	constraints[1].max_total_length =7.0;
	constraints[1].max_curvature =0.15;				
	constraints[1].max_rough =0.5;
	constraints[1].max_growth_length =0.4;
	constraints[1].max_growth_radius =0.04;
	constraints[1].min_branch_distance =2;
	
	constraints[2].max_extensions = 3;	
	constraints[2].min_extensions = 2;	
	constraints[2].sections =6;		
	constraints[2].steps =3;
	constraints[2].max_radius =0.15;
	constraints[2].min_radius =0.08;
	constraints[2].max_length =0.4;
	constraints[2].min_length =0.2;
	constraints[2].max_total_length =3.0;
	constraints[2].max_curvature =0.1;				
	constraints[2].max_rough =0.0;
	constraints[2].max_growth_length =0.4;
	constraints[2].max_growth_radius =0.04;
	constraints[2].min_branch_distance =1;
	
	constraints[3].max_extensions = 1;	
	constraints[3].min_extensions = 1;	
	constraints[3].sections =3;		
	constraints[3].steps =3;
	constraints[3].max_radius =0.08;
	constraints[3].min_radius =0.05;
	constraints[3].max_length =0.4;
	constraints[3].min_length =0.2;
	constraints[3].max_total_length =3.0;
	constraints[3].max_curvature =0.05;				
	constraints[3].max_rough =0.0;
	constraints[3].max_growth_length =0.4;
	constraints[3].max_growth_radius =0.04;
	constraints[3].min_branch_distance =1;
	
	m_up = glm::vec3( 0, 1, 0 );
	m_wind_dir = glm::vec3( 1, 0, 0);
	m_wind_freq_theta =0.0f;
	m_wind_theta =0.0f;

	root =createNode(0);
	root->up = m_up;
	root->max_branches =0;
	
	//
	
	geom.setLeafMode( PTreeGeometry::LEAF );
	
	season =SEASON_SPRING;
	season_time =0;
	time_per_season = 20;
	gravity =0.9;
	
	geom.updateBuffers();
}

PTree::~PTree() {
}

void 
PTree::setWireMode( bool w ) {
	geom.setWireMode( w );
}

void 
PTree::reinitialize() {

	std::list<Leaf*>::iterator it;
	for( it = leafs.begin(); it != leafs.end(); it ++ )
		delete (*it);
	leafs.clear();
	geom.clearLeafs();
	
	cleanupRecursive( root );
	geom.clearBranches();
	
	root =createNode(0);
	root->up = m_up;
	root->max_branches =0;
	season =SEASON_SPRING;
	season_time =0;
	
	geom.updateBuffers();
}

Geometry* 
PTree::geometry( int ) {
	return &geom;
}

void 
PTree::update( float deltatime ) {

	deltatime = glm::min( deltatime, 0.1f );
	
	season_time += deltatime;
	if( season_time > time_per_season ) {
		season_time =0;
		/* Change the season */
		if( season == SEASON_WINTER ) {
			season = SEASON_SPRING;
			/* Cleanup the fallen leafs */
			std::list<Leaf*>::iterator it;
			for( it = leafs.begin(); it != leafs.end(); it ++ )
				delete (*it);
			leafs.clear();
			geom.clearLeafs();
		}
		else {
			season++;
		}
	}
	
	growRecursive( 0, root, 0, deltatime );
	growLeafs( deltatime );
	
	m_wind_freq_theta += 0.1;
	m_wind_theta += 2.0 * deltatime;
	m_wind_dir =glm::rotate( m_wind_dir, (float)glm::linearRand( 0.0f, 0.01f ), m_up );
	
	geom.setWind( glm::normalize(m_wind_dir) * sinf( m_wind_theta ),
								glm::normalize(m_wind_dir) * sinf( 2 * m_wind_theta ) );
	
	geom.updateBuffers();
	Actor::update( deltatime );
}

PTree::Node::Node() 
	: index( 0 ),
		level( 0 ),
		extension( 0 ),
		branches( 0 ),
		leaf( 0 )
		{ }
		
void 
PTree::growRecursive( Node* parent, Node* n, int extension_count, float deltatime ) {
	
	if( !n )
		return;
		
	const Constraint& c= constraints[n->level];
	

	if( parent ) {
		if( extension_count )
			data(n).setAttrib( PBranchSection::BASE_ROUGH,
				data(parent).data[PBranchSection::HEAD_ROUGH] );
		data(n).setAttrib( PBranchSection::BASE_RADIUS, 
			data(parent).data[PBranchSection::HEAD_RADIUS] * (!extension_count ? 0.7 : 1.0) );
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
	
	glm::vec3 up = n->up;
	
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
	&& !n->extension  ) {
	
		if( extension_count < n->max_extensions ) {
			/* Grow an extension... */
			n->extension =createNode( n->level );
		
			if( extension_count < c.min_branch_distance ) 
				n->extension->branches =0;		
		
			float max_angle = ( M_PI / 4.0f ) * ( (float)(n->level + 1)/ (float)MAX_LEVEL );
			float min_angle = max_angle * 0.8;
			glm::vec3 axis = glm::cross( data(n).attribVector( PBranchSection::HEAD ) - n->curve, up );
			axis = glm::rotate( axis, (float)glm::linearRand( 0.0, 2* M_PI ), up );
			float angle = glm::linearRand( min_angle, max_angle );	
			glm::vec3 offset = glm::rotate( up, angle, axis );
		
			glm::vec3 branch_up = glm::normalize(offset);
	
			data(n->extension).setAttrib( PBranchSection::HEAD, data(n).attribVector( PBranchSection::HEAD ) + branch_up * 0.01f );
			data(n->extension).setAttrib( PBranchSection::BASE_SEED, data(n).data[PBranchSection::HEAD_SEED] );
			data(n->extension).setAttrib( PBranchSection::PREV_SEED, data(n).data[PBranchSection::BASE_SEED] );
			data(n->extension).setAttrib( PBranchSection::HEAD_SEED, data(n).data[PBranchSection::HEAD_SEED] );

			n->extension->max_radius = glm::max( n->max_radius * 0.85f, 0.05f );
			n->extension->max_extensions =n->max_extensions;
			n->extension->up = branch_up;
		} else if( !n->leaf ) {
			/* ... Or grow a leaf */
			if( season == SEASON_SPRING ) {
				n->leaf = createLeaf( n );
				data(n->leaf).setAttrib( PLeaf::TIP, data(n).attribVector( PBranchSection::HEAD ) );
			}
		}
	}

		
	if( n->extension ) {
		float head_radius =data(n).data[PBranchSection::HEAD_RADIUS];
		if( head_radius < n->max_radius ) {
			head_radius += glm::linearRand( 0.0f, c.max_growth_radius ) * deltatime;
			//head_radius += c.max_growth_radius * deltatime;
			data(n).setAttrib( PBranchSection::HEAD_RADIUS, head_radius );
		}
	}
	else
		data(n).setAttrib( PBranchSection::HEAD_RADIUS, 0.001f );
	
	if( data(n).data[ PBranchSection::HEAD_RADIUS ] > n->max_radius * 0.15
		&& n->level != MAX_LEVEL -1
		&& n->branches < n->max_branches ) {
		
		int b = n->branches++;
		
		n->branch[b] =createNode( n->level + 1 );
		
		if( extension_count < c.min_branch_distance ) 
			n->branch[b]->branches =0;
			

		float max_angle = ( M_PI / 2.0f ) * ( 1.1 - (float)(extension_count + 1)/ (float)c.max_extensions );
		float min_angle = max_angle * 0.9;
		
		//glm::vec3 axis = glm::cross( data(n).attribVector( PBranchSection::HEAD ), data(n).attribVector( PBranchSection::BASE ) );
		glm::vec3 axis = glm::cross( data(n).attribVector( PBranchSection::HEAD ) - n->curve, up );
		axis = glm::rotate( axis, (float)glm::linearRand( 0.0, 2* M_PI ), up );
		float angle = glm::linearRand( min_angle, max_angle );	
		glm::vec3 offset = glm::rotate( up, angle, axis );
		
		data(n->branch[b]).setAttrib( PBranchSection::HEAD, data(n).attribVector( PBranchSection::HEAD ) + offset * 0.01f );
		data(n->branch[b]).setAttrib( PBranchSection::BASE_SEED, data(n).data[PBranchSection::HEAD_SEED] );
		data(n->branch[b]).setAttrib( PBranchSection::PREV_SEED, data(n).data[PBranchSection::BASE_SEED] );
		data(n->branch[b]).setAttrib( PBranchSection::HEAD_SEED, glm::linearRand( 0.0, 1.0 ) );
		n->branch[b]->max_radius *= glm::max( /*n->branch[b]->max_radius*/ 0.05f, n->max_radius * 0.8f );
		n->branch[b]->up = offset;
		
	}
	
	float max_rough, rough;
	if( parent && extension_count )
		max_rough =data(parent).data[PBranchSection::HEAD_ROUGH] * 0.8f;
	else
		max_rough =c.max_rough;
		
		
		
	if( (rough = data(n).data[PBranchSection::HEAD_ROUGH]) < max_rough ) {
		rough += glm::linearRand( 0.0f, c.max_growth_radius ) * deltatime;
		data(n).setAttrib( PBranchSection::HEAD_ROUGH, rough );
		if( !parent || !extension_count )
			data(n).setAttrib( PBranchSection::BASE_ROUGH, rough );
	}
	
	data(n).setAttrib( PBranchSection::CURVE, n->curve * data(n).length() / n->max_length );
	
	growRecursive( n, n->extension, extension_count+1, deltatime );
	for( int b =0; b < n->branches; b++ )
		growRecursive( n, n->branch[b], 0, deltatime );
	
}

void
PTree::growLeafs( float deltatime ) {

	std::list<Leaf*>::iterator it;
	for( it = leafs.begin(); it != leafs.end(); it ++ ) {
		Leaf* l = (*it);
		/* Sync with parent position, only if we're still attached :-) */
		if( season == SEASON_SPRING || season == SEASON_SUMMER ) {
			glm::vec3 base = data(l->parent).attribVector( PBranchSection::HEAD );
			glm::vec3 tip = data(l).attribVector( PLeaf::TIP );
			
			/* Account for the latest growth of our parent */
			tip += l->parent->last_growth;
			
			/* Grow depending on size */
			if( data(l).length() < 0.07f )
				tip += glm::normalize( l->parent->up ) * 0.25f * deltatime;
				
			data(l).setAttrib( PLeaf::BASE, base );
			data(l).setAttrib( PLeaf::TIP, tip );
			data(l).setAttrib( PLeaf::UP, glm::cross( data(l->parent).attribVector( PBranchSection::HEAD ),
			 data(l->parent).attribVector( PBranchSection::BASE ) ) );
		}
		else
			l->parent->leaf =0;
		
		if( season == SEASON_AUTUMN ) {
			
			/* Let's fall, determine the down direction first */
			glm::vec3 down = glm::normalize( -m_up + m_wind_dir );
			
			/* Only fall if not on the ground already */			
			if( glm::any( glm::greaterThan( data(l).attribVector( PLeaf::BASE ) * m_up, glm::vec3(0.0f) ) ) ) {
			
				/* Take acceleration into account */
				l->speed += gravity * deltatime;
			
				glm::vec3 base = data(l).attribVector( PLeaf::BASE ) + down * l->speed * deltatime;
				glm::vec3 tip = data(l).attribVector( PLeaf::TIP ) + down * l->speed * deltatime;
			
				data(l).setAttrib( PLeaf::BASE, base );
				data(l).setAttrib( PLeaf::TIP, tip );
			}
		
		} else if ( season == SEASON_WINTER ) {
			float width = data(l).data[ PLeaf::WIDTH ];
			glm::vec3 base = data(l).attribVector( PLeaf::BASE );
			glm::vec3 tip = data(l).attribVector( PLeaf::TIP );
			data(l).setAttrib( PLeaf::WIDTH, glm::max( 0.0f, width - deltatime * 0.5f ) );
			data(l).setAttrib( PLeaf::TIP, base * 0.01f + tip * 0.99f );
			data(l).setAttrib( PLeaf::BASE, base * 0.99f + tip * 0.01f );
			
		}
		
	}

}

void 
PTree::cleanupRecursive( Node* n ) {

	if( !n )
		return;
		
	cleanupRecursive( n->extension );
	for( int i =0; i < n->branches; i++ )
		cleanupRecursive( n->branch[i] );
		
	delete n;
}

PTree::Node*
PTree::createNode( int level ) {
	PTree::Node *n =new PTree::Node();
	int i =geom.append( PBranchSection() );
	n->index =i;
	n->age =0;
	n->level =level;
	n->max_length = glm::linearRand( constraints[level].min_length, constraints[level].max_length );
	n->max_radius = glm::linearRand( constraints[level].min_radius, constraints[level].max_radius );
	n->curve = glm::ballRand( constraints[level].max_curvature ) + glm::vec3(0.01f);
	n->max_branches = glm::linearRand( 1.0, 3.4 );
	n->max_extensions = glm::linearRand( constraints[level].min_extensions, constraints[level].max_extensions );
	
	data(n).setAttrib( PBranchSection::SECTIONS, constraints[level].sections );
	data(n).setAttrib( PBranchSection::STEPS, constraints[level].steps );
	
	return n;
}

PTree::Leaf*
PTree::createLeaf( Node* parent ) {

	PTree::Leaf *l =new PTree::Leaf();
	int i =geom.append( PLeaf() );
	l->index =i;
	l->parent =parent;
	l->speed =0;
	
	data(l).setAttrib( PLeaf::WIDTH, 0.15 );
	data(l).setAttrib( PLeaf::CONCAVITY, 0.01 );
	data(l).setAttrib( PLeaf::NUM_LEAFS, 5 );
	
	data(l).setAttrib( PLeaf::SEED, data(parent).data[PBranchSection::HEAD_SEED] );
	
	leafs.push_back( l );
	return l;
}

PBranchSection&
PTree::data( Node* n ) {
	return geom.branchAt( n->index );
}

PLeaf&
PTree::data( Leaf* n ) {
	return geom.leafAt( n->index );
}