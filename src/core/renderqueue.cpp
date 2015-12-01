#include "renderqueue.h"
#include "geometry.h"
#include "object.h"
#include "actor.h"

#include <stack>

RenderQueue::RenderQueue() : m_root( 0 )
{}
		
void 
RenderQueue::rebuild( /*glm::mat4 viewmatrix*/ ) {

	m_queue.clear();
	
	// Traverse the object-tree in a DFS manner
	
	std::stack<Object*> obj_stack;
	std::stack<glm::mat4> mat_stack;
	obj_stack.push( m_root );
	mat_stack.push( modelMat( m_root ) );
	
	while( !obj_stack.empty() ) {
		
		Object *o = obj_stack.top();
		glm::mat4 matmodel = mat_stack.top();
		obj_stack.pop();
		mat_stack.pop();
			
		// Only actor types contain drawable geometry
		
		if( o->isA( Actor::TYPE ) ) {
			Actor *actor = reinterpret_cast<Actor*>(o);
			
			if( actor->geometry() ) {
				RenderQueueTuple tuple;
					
				tuple.matmodel = matmodel;
				
				tuple.geometry = actor->geometry();
			
				m_queue.push_back( tuple );			
			}
		}
			
		Object::Objects objs =o->getChildren();
		
		if( !objs.empty() ) {
			for( Object::Objects::iterator it =objs.begin();it !=objs.end(); it++ ){
				obj_stack.push( (*it) );
				mat_stack.push( matmodel * modelMat(*it)  );
			}
		}
		
	}

}

glm::mat4 
RenderQueue::modelMat( Object* obj ) {

	//glm::mat4 mat = glm::translate( obj->position() );
	
	return obj->transform().matrix();
}