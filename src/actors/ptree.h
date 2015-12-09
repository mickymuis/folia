#ifndef PTREE_H
#define PTREE_H

#include "../common/platform.h"
#include "../core/actor.h"
#include "../core/geometry.h"
#include "../utils/glm/vec3.hpp"
#include <vector>
#include <list>

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
			SECTIONS,		STEPS,				PREV_SEED,
			NUM_FLOATS
		};

		void setAttrib( Attrib, glm::vec3 );
		void setAttrib( AttribElement, float );
		glm::vec3 attribVector( Attrib ) const;
			
		float length() const;
		
		GLfloat data[NUM_FLOATS];
		
		PBranchSection();
};

class PLeaf {
	public:
		enum Attrib {
			BASE,
			PARAMS1,
			TIP,
			PARAMS2,
			UP,
			UNUSED
		};

		enum AttribElement {
			BASE_X,		BASE_Y,		BASE_Z,
			WIDTH,		CONCAVITY,SEED,
			TIP_X,		TIP_Y,		TIP_Z,
			NUM_LEAFS,ROUNDESS, RESERVED,
			UP_X,			UP_Y,			UP_Z,
			RESERVED1,RESERVED2,RESERVED3,
			NUM_FLOATS
		};

		void setAttrib( Attrib, glm::vec3 );
		void setAttrib( AttribElement, float );
		glm::vec3 attribVector( Attrib ) const;
			
		float length() const;
		
		GLfloat data[NUM_FLOATS];
		
		PLeaf();
};

class PTreeGeometry : public Geometry {
	public:
		enum LeafMode {
			LEAF,
			BLOSSOM
		};
		
		PTreeGeometry();
		~PTreeGeometry();
		
		size_t append( const PBranchSection& );
		size_t append( const PLeaf& );
		
		PBranchSection& branchAt( int index );
		PLeaf& leafAt( int index );
		
		void clearLeafs();
		void clearBranches();
		
		void updateBuffers();
		void preRender( int n =0 );
		
		size_t branchCount() const;
		size_t leafCount() const;
		bool renderDeferred( int n = 1 ) const { return true; }
		
		void setWind( glm::vec3, glm::vec3 );
		
		void setLeafMode( LeafMode );
		
		void setWireMode( bool );
		
		int bufferCount() const { return 2; }
		
		GLuint vao( int n = 0 ) const;
		ShaderProgram* program( int n = 0 ) const;
		GLenum type( int n = 0 ) const;
		GLint first( int n = 0 ) const;
		GLsizei size( int n = 0 ) const;
		
	private:
		void initBuffers();
		void destroyBuffers();
		
		GLuint vbo_branches;
		GLuint vbo_leafs;
		GLuint m_vao_leafs;
		GLuint tex_diffuse1, tex_diffuse2;
		GLuint tex_normal;
		
		std::vector<PBranchSection> branches;
		std::vector<PLeaf> leafs;
		glm::vec3 wind_1, wind_2;
		
		LeafMode leaf_mode;
		
		ShaderProgram *m_wireProgram;
		ShaderProgram *m_branchProgram;
		ShaderProgram *m_leafProgram;
		ShaderProgram *m_blossomProgram;
};

class PTree : public Actor {
	public:
		PTree( Object* parent );
		~PTree();
		
		void setWireMode( bool );
		void reinitialize();
		
		Geometry* geometry(int n =0);
		int geometryCount() { return 1; }
		
		virtual void update( float deltatime );
		
	private:
		enum Constants {
			MAX_LEVEL =4
		};
		enum Season {
			SEASON_SPRING,
			SEASON_SUMMER,
			SEASON_AUTUMN,
			SEASON_WINTER
		};
		class Constraint {
			public:
				int max_extensions;	
				int min_extensions;
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
		
		class Leaf;
		class Node {
			public:
				int index;
				int level;
				int branches;
				int max_branches;
				int max_extensions;
				
				float age;
				float totalLength;
				float max_radius;
				float max_length;
				
				glm::vec3 curve;
				glm::vec3 last_growth;
				glm::vec3 up;
				
				Node* extension; // left child
				Node* branch[3]; // right child(ren)
				Leaf* leaf;
				
				Node();
		};
		
		class Leaf {
			public:
				int index;
				float speed;
				
				Node *parent;
		};
		
		void cleanupRecursive( Node* );
		void growRecursive( Node* parent, Node* n, int extension_count, float deltatime );
		void growLeafs( float deltatime );
		PBranchSection &data( Node* );
		PLeaf &data( Leaf* );
		Node *createNode( int level );
		Leaf *createLeaf( Node* parent );
	
		PTreeGeometry geom;
		Node *root;
		Constraint constraints[MAX_LEVEL];
		glm::vec3 m_up ;
		glm::vec3 m_wind_dir;
		float m_wind_freq_theta;
		float m_wind_theta;
		
		int season;
		float season_time;
		float time_per_season;
		float gravity;
		
		std::list<Leaf*> leafs;
};

#endif