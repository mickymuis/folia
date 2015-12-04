#ifndef PTREE_H
#define PTREE_H

#include "../common/platform.h"
#include "../core/actor.h"
#include "../core/geometry.h"
#include "../utils/glm/vec3.hpp"
#include <vector>

class PBranchSection {
	public:
		enum Attrib {
			PREV,
			BASE_PARAMS,
			BASE,
			HEAD_PARAMS,
			HEAD,
			CURVE,
			NEXT,
			PARAMS
		};

		enum AttribElement {
			PREV_X,			PREV_Y,				PREV_Z,
			BASE_RADIUS,BASE_ROUGH,		BASE_SEED,
			BASE_X,			BASE_Y,				BASE_Z,
			HEAD_RADIUS,HEAD_ROUGH,		HEAD_SEED,
			HEAD_X,			HEAD_Y,				HEAD_Z,
			CURVE_X,		CURVE_Y,			CURVE_Z,
			NEXT_X,			NEXT_Y,				NEXT_Z,
			SECTIONS,		STEPS,				RESERVED,
			NUM_FLOATS
		};

		void setAttrib( Attrib, glm::vec3 );
		void setAttrib( AttribElement, float );
		glm::vec3 attribVector( Attrib ) const;
		
		float length() const;
		
		GLfloat data[NUM_FLOATS];
		
		PBranchSection();
};

class PTreeGeometry : public Geometry {
	public:
		PTreeGeometry();
		~PTreeGeometry();
		
		size_t append( int num =1 );
		size_t append( const PBranchSection& );
		
		void setDataAt( int index, const PBranchSection& );
		PBranchSection& dataAt( int index );
		
		void updateBuffers();
		
		size_t branchCount() const;
		virtual GLsizei size() const { return branchCount() * 4; } 
		virtual bool renderDeferred() const { return true; }
		
	private:
		void initBuffers();
		void destroyBuffers();
		
		GLuint vbo_position;
		
		std::vector<PBranchSection> data;
};

class PTree : public Actor {
	public:
		PTree( Object* parent );
		~PTree();
		Geometry* geometry();
		
		virtual void update( float deltatime );
		
	private:
		enum Constants {
			MAX_LEVEL =3
		};
		class Constraint {
			public:
				int max_extensions;	
				int sections;		
				int steps;
				int min_branch_distance;
				float max_radius;
				float min_radius;
				float max_length;
				float min_length;
				float max_total_length;
				float max_curvature;				
				float max_rough;
				float max_growth_length;
				float max_growth_radius;
		};
		
		class Node {
			public:
				int index;
				int level;
				int branches;
				
				float age;
				float totalLength;
				float max_radius;
				float max_length;
				
				glm::vec3 curve;
				glm::vec3 last_growth;
				glm::vec3 up;
				
				Node* extension; // left child
				Node* branch; // right child
				
				Node();
		};
		
		void growRecursive( Node* parent, Node* n, int extension_count, float deltatime );
		PBranchSection &data( Node* );
		Node *createNode( int level );
		
	
		PTreeGeometry geom;
		Node *root;
		Constraint constraints[MAX_LEVEL];
};

#endif