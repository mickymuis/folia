#ifndef TENDRILFIELD_H
#define TENDRILFIELD_H

#include "../common/platform.h"
#include "../core/actor.h"
#include "../core/geometry.h"
#include "../utils/glm/vec3.hpp"
#include <vector>


class Tendril {
	public:
		enum Attrib {
			BASE,
			COLOR,
			SHOULDER,
			PARAMS,
			HEAD,
			PARAMS2
		};

		enum AttribElement {
			BASE_X,			BASE_Y,				BASE_Z,
			COLOR_R,		COLOR_G,			COLOR_B,
			SHOULDER_X,	SHOULDER_Y,		SOULDER_Z,
			SECTIONS,		TAPER,				STEP,
			HEAD_X,			HEAD_Y,				HEAD_Z,
			THICKNESS,	CURL,					GROWTH,
			NUM_FLOATS
		};

		void setAttrib( Attrib, glm::vec3 );
		void setAttrib( AttribElement, float );
		glm::vec3 attribVector( Attrib ) const;
		
		GLfloat data[NUM_FLOATS];
		
		Tendril();
};

class TendrilGeometry : public Geometry {
	public:
		TendrilGeometry();
		~TendrilGeometry();
		
		size_t append( int num =1 );
		size_t append( const Tendril& );
		
		void setDataAt( int index, const Tendril& );
		Tendril& dataAt( int index );
		
		void updateBuffers();
		
		size_t tendrilCount() const;
		virtual GLsizei size( int n = 0 ) const { return tendrilCount() * 3; } 
		virtual bool renderDeferred( int n = 0 ) const { return true; }
		
	private:
		void initBuffers();
		void destroyBuffers();
		
		GLuint vbo_position;
		GLuint vbo_param;
		
		std::vector<Tendril> data;
};

class TendrilField : public Actor {
	public:
		TendrilField( Object* parent );
		~TendrilField();
		Geometry* geometry( int n =0 );
		
		virtual void update( float deltatime );
		
	private:
		TendrilGeometry geom;
};

#endif