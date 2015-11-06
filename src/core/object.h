#ifndef OBJECT_H
#define OBJECT_H

#include <set>
#include <cinttypes>
#include "../utils/glm/vec3.hpp"
#include "geometry.h"

class Object {
	public:
		Object( Object* parent =0 );
		~Object();
	
		Object* getParent( ) const;
		virtual void update( float deltatime );
		
		typedef std::set<Object*> Objects;
		Objects getChildren( ) const;
		
		void setTransform( const Transformation& t ) { m_transform =t; }
		Transformation& transform() { return m_transform; }
			
		virtual int RTTI() const =0;
		inline bool isA( int type ) const { return type == RTTI(); }
	
	protected:
		Objects children;
		Object* parent;
	
	private:
		Transformation m_transform;
};

#endif