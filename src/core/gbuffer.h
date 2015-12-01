#ifndef GBUFFER_H
#define GBUFFER_H

#include "../common/platform.h"
#include "window.h"

class GBuffer {
	public:
	
		enum Texture {
			DEPTH_TEXTURE,
			DIFFUSE_SPEC_TEXTURE,
			NORMAL_TEXTURE,
			NUM_TEXTURES
		};
		
		GBuffer();
		~GBuffer();
		
		bool initialize( const Viewport& );
		void destroy();
		
		void bindForReading();
		void bindForWriting();
		
		int width() const { return m_width; }
		int height() const { return m_height; }
		
	private:
		
		GLuint m_fbo;
		GLuint m_textures[NUM_TEXTURES];
		int m_width;
		int m_height;
	
};

#endif