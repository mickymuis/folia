/*
 * This file is part of Folia, an experimental mini-engine using OpenGL 3
 * Created by Micky Faas. Freely usable and modifiable for academic purposes.
 */

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
			POSITION_TEXTURE,
			NUM_TEXTURES
		};
		
		GBuffer();
		~GBuffer();
		
		bool initialize( const Viewport&, bool multisample, int samples );
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
		bool m_multisample;
	
};

#endif