/*
 * This file is part of Folia, an experimental mini-engine using OpenGL 3
 * Created by Micky Faas. Freely usable and modifiable for academic purposes.
 */

#ifndef RENDER_QUEUE_H
#define RENDER_QUEUE_H

#include <vector>
#include "../utils/glm/mat4x4.hpp"

class Geometry;
class Object;
class Actor;

class RenderQueueTuple {
	public:
		Geometry *geometry;
		glm::mat4 matmodel;
		
};

class RenderQueue {
	public:
		typedef std::vector<RenderQueueTuple> queue_t;
		
		RenderQueue();
		
		void setRootObject( Object* o ) { m_root =o; }
		Object* rootObject() const { return m_root; }
		
		void setFilters( int f ) { m_filters = f; }
		int filters() const { return m_filters; }
		
		void rebuild( /*glm::mat4 viewmatrix*/ );
		
		const queue_t &queue() const { return m_queue; }
		
	private:
		queue_t m_queue;
		Object* m_root;
		int m_filters;
		
		glm::mat4 modelMat( Object* );

};

#endif