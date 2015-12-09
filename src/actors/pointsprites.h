#ifndef POINTSPRITES_H
#define POINTSPRITES_H

#include "../common/platform.h"
#include "../core/actor.h"
#include "../core/geometry.h"
#include "../utils/glm/vec3.hpp"
#include <vector>

class PointSpritesGeometry : public Geometry {
	public:
		struct Point{
			glm::vec3 position;
			glm::vec3 params;
		};
		typedef std::vector<Point> Points;
		
		PointSpritesGeometry();
		~PointSpritesGeometry();
		
		virtual bool renderDeferred( int n = 0 ) const { return false; }
		virtual GLsizei size( int n = 0 ) const { return m_data.size(); } 
		
		void setProgram( ShaderProgram* );
		void updateBuffers();
		
		Points& data() { return m_data; }
		const Points& data() const { return m_data; }
		
	private:
		void initBuffers();
		void destroyBuffers();
		
		GLuint vbo_points;
		
//		glm::vec3 view_position;
//		glm::vec3 current_view_position;
		Points m_data;
};

class PointSprites : public Actor {
	public:
		PointSprites( Object* parent );
		~PointSprites();
		
		int append( glm::vec3 position, float radius =1.0f );
		void remove( int );
		
		void setPosition( int i, glm::vec3 );
		void setRadius( int i, float );
		
		int count() const;
		
		void setProgram( ShaderProgram* );
		
		Geometry* geometry( int n =0 );
		
	private:
		PointSpritesGeometry geom;
};

#endif