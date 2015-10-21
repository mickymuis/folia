#ifndef ENGINE_H
#define ENGINE_H

#include <cinttypes>
#include "../utils/glm/fwd.hpp"

class Viewport;
class Object;

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
		Engine( int enable =FT_DEPTHBUFFER );
		~Engine();
	
		void enableFeature( Feature );
		void disableFeature( Feature );
		bool isEnabledFeature( Feature );
	
		void draw( Viewport* );
	
	private:
		// Render passes
		void preRender( Viewport* );
		
		// Initializers
		void initFeatures();
		
		// Utility functions
		glm::mat4 modelMat( Object* );
		
		// Member variables
		int m_features;
};

#endif
