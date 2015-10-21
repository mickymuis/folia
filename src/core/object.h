#ifndef OBJECT_H
#define OBJECT_H

#include <set>
#include <cinttypes>
#include "../utils/glm/vec3.hpp"

class Object {
	public:
		Object( Object* parent =0 );
		~Object();
	
		Object* getParent( ) const;
		virtual void update( uint32_t deltatime );
		
		typedef std::set<Object*> Objects;
		Objects getChildren( ) const;
		
		void setPosition( glm::vec3 p ) { m_position = p; }
		glm::vec3 position() const { return m_position; }
		
		virtual int RTTI() const =0;
		inline bool isA( int type ) const { return type == RTTI(); }
	
	protected:
		Objects children;
		Object* parent;
	
	private:
		glm::vec3 m_position;
};

#endif