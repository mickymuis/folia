#ifndef ENGINE_H
#define ENGINE_H

#include <cinttypes>
#include "../utils/glm/fwd.hpp"
#include "../utils/glm/mat4x4.hpp"
#include "gbuffer.h"
#include "renderqueue.h"
#include "geometry.h"

class Viewport;
class Object;

class ScreenQuad : public Geometry {
	public:
		ScreenQuad();

		bool initialize();
		void destroy();
	private:
		
		GLuint vbo_position;
		static GLfloat data[20];
};

class Engine {
	public:
		enum Feature {
			FT_DEPTHBUFFER =0x1,
			FT_ALPHABLEND =0x2,
			FT_LIGHTING =0x4,
			FT_AA =0x8,
			FT_POSTPROCESS =0x10,
			FT_SHADOWMAP =0x20
		};
		
		enum Filter {
			NONE =0x0,
			DEFERRABLE =0x1,
			FORWARD_ONLY =0x2,
			ALL =0x3
		};
		
		Engine( int enable =FT_DEPTHBUFFER );
		~Engine();
	
		void enableFeature( Feature );
		void disableFeature( Feature );
		bool isEnabledFeature( Feature );
	
		void draw( Viewport* );
	
	private:
		// Render passes
		void preRender( Viewport* );
		void renderQueue( const RenderQueue&, glm::mat4 viewmatrix, glm::mat4x4 matprojection, int filters );
		void render( Geometry* );
		
		// Initializers
		void initialize();
		void initFeatures();
		
		// Utility functions
		bool filter( Geometry*, int filters );
		
		// Member variables
		bool m_hasError;
		int m_features;
		RenderQueue m_queue;
		GBuffer m_gbuffer;
		ScreenQuad m_screen;
};

#endif
