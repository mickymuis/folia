#ifndef TENDRILFIELD_H
#define TENDRILFIELD_H

#include "../common/platform.h"
#include "../core/actor.h"
#include "../core/geometry.h"
#include "../utils/glm/vec3.hpp"
#include <vector>

enum TendrilVectorAttribute {
	TENDRIL_BASE =0,
	TENDRIL_SHOULDER =1,
	TENDRIL_HEAD =2,
	TENDRIL_COLOR =3,
	TENDRIL_COLOR2 =4,
	TENDRIL_PARAMS =5
};

enum TendrilFloatAttribute {
	TENDRIL_BASE_X,			TENDRIL_BASE_Y,				TENDRIL_BASE_Z,
	TENDRIL_SHOULDER_X,	TENDRIL_SHOULDER_Y,		TENDRIL_SOULDER_Z,
	TENDRIL_HEAD_X,			TENDRIL_HEAD_Y,				TENDRIL_HEAD_Z,
	TENDRIL_COLOR_R,		TENDRIL_COLOR_G,			TENDRIL_COLOR_B,
	TENDRIL_COLOR2_R,		TENDRIL_COLOR2_G,			TENDRIL_COLOR2_B,
	TENDRIL_THICKNESS,	TENDRIL_CURL,					TENDRIL_PARAM,
	TENDRIL_NUM_FLOATS
};

class TendrilGeometry : public Geometry {
	public:
		TendrilGeometry();
		~TendrilGeometry();
		
		int append( int num =1 );
		
		void setAttribute( int index, TendrilAttribute, glm::vec3 );
		void updateBuffers();
		
		int tendrilCount();
		
	private:
		void initBuffers();
		void destroyBuffers();
		
		GLuint vbo_position;
		GLuint vbo_param;
		
		vector<GLfloat> data;
};

class TendrilField : public Actor {
	public:
		TendrilField( Object* parent );
		~TendrilField();
		Geometry* geometry();
		
	private:
		TendrilGeometry geom;
};

#endif