/*
 * This file is part of Folia, an experimental mini-engine using OpenGL 3
 * Created by Micky Faas. Freely usable and modifiable for academic purposes.
 */

#include "gbuffer.h"
#include <stdio.h>

GBuffer::GBuffer() : m_fbo(0), m_width(0), m_height(0) {}

GBuffer::~GBuffer() { destroy(); }

bool 
GBuffer::initialize( const Viewport& vp, bool multisample, int samples ) {

	int width =vp.width - vp.x;
	int height =vp.height - vp.y;

	/* First time initialize? */
	if( !m_fbo ) {
		glEnable( GL_MULTISAMPLE );
		glGenFramebuffers(1, &m_fbo); 
	}
	else {
		glDeleteTextures( NUM_TEXTURES, m_textures);
	}
	
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fbo);
//	glRenderbufferStorageMultisample( GL_RENDERBUFFER, 2, GL_DEPTH24_STENCIL8, width, height );
	glGenTextures( NUM_TEXTURES, m_textures );
	
	/* Depth */
	
	if( multisample ) {
		glBindTexture( GL_TEXTURE_2D_MULTISAMPLE, m_textures[DEPTH_TEXTURE] );
		glTexImage2DMultisample( GL_TEXTURE_2D_MULTISAMPLE, samples, GL_DEPTH_COMPONENT32F, width, height, false );
/*		glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE, GL_LUMINANCE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);*/
		glFramebufferTexture2D( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D_MULTISAMPLE, m_textures[DEPTH_TEXTURE], 0 );
  } else {
		glBindTexture(GL_TEXTURE_2D, m_textures[DEPTH_TEXTURE]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT,
									NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_textures[DEPTH_TEXTURE], 0);
	}	
  
	
	/* Diffuse + spec */
	if( multisample ) {
	  glBindTexture( GL_TEXTURE_2D_MULTISAMPLE, m_textures[DIFFUSE_SPEC_TEXTURE] );
  	glTexImage2DMultisample( GL_TEXTURE_2D_MULTISAMPLE, samples, GL_RGBA16F, width, height, false );
  	glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, m_textures[DIFFUSE_SPEC_TEXTURE], 0 );
  } else {
		glBindTexture(GL_TEXTURE_2D, m_textures[DIFFUSE_SPEC_TEXTURE]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_textures[DIFFUSE_SPEC_TEXTURE], 0);
  }
  
/*  printf("GL error, status: 0x%x\n", glGetError());*/
	
	/* Normals */
	if( multisample ) {
		glBindTexture( GL_TEXTURE_2D_MULTISAMPLE, m_textures[NORMAL_TEXTURE] );
		glTexImage2DMultisample( GL_TEXTURE_2D_MULTISAMPLE, samples, GL_RGB16F, width, height, false );
		glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D_MULTISAMPLE, m_textures[NORMAL_TEXTURE], 0 );
	} else {	
		glBindTexture(GL_TEXTURE_2D, m_textures[NORMAL_TEXTURE]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_textures[NORMAL_TEXTURE], 0);
  }
  
  /* Position */
	if( multisample ) {
		glBindTexture( GL_TEXTURE_2D_MULTISAMPLE, m_textures[POSITION_TEXTURE] );
		glTexImage2DMultisample( GL_TEXTURE_2D_MULTISAMPLE, samples, GL_RGBA16F, width, height, false );
		glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D_MULTISAMPLE, m_textures[POSITION_TEXTURE], 0 );
	} else {	
		glBindTexture(GL_TEXTURE_2D, m_textures[POSITION_TEXTURE]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, m_textures[POSITION_TEXTURE], 0);
  }
	
	GLenum DrawBuffers[] = { /*GL_DEPTH_ATTACHMENT, */GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2/*, GL_COLOR_ATTACHMENT3 */}; 
  glDrawBuffers(NUM_TEXTURES-1, DrawBuffers);

	GLenum Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

    if (Status != GL_FRAMEBUFFER_COMPLETE) {
        printf("FB error, status: 0x%x\n", Status);
        return false;
    }
    
  m_width =width;
  m_height =height;
  m_multisample =multisample;
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	return true;
}

void
GBuffer::destroy() {

	if( m_fbo ) {
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		glDeleteTextures( NUM_TEXTURES, m_textures);
		glDeleteFramebuffers(1, &m_fbo);
		
		m_fbo =0;
	}
}
		
void 
GBuffer::bindForReading() {
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glBindFramebuffer(GL_READ_FRAMEBUFFER, m_fbo );
	
	glActiveTexture(GL_TEXTURE0);
	if( m_multisample )
		glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_textures[DEPTH_TEXTURE]);
	else
		glBindTexture(GL_TEXTURE_2D, m_textures[DEPTH_TEXTURE]);
	
	glActiveTexture(GL_TEXTURE1);
	if( m_multisample )
		glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_textures[DIFFUSE_SPEC_TEXTURE]);
	else
		glBindTexture(GL_TEXTURE_2D, m_textures[DIFFUSE_SPEC_TEXTURE]);
		
	glActiveTexture(GL_TEXTURE2);
	if( m_multisample )
		glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_textures[NORMAL_TEXTURE]);
	else
		glBindTexture(GL_TEXTURE_2D, m_textures[NORMAL_TEXTURE]);
		
	glActiveTexture(GL_TEXTURE3);
	if( m_multisample )
		glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_textures[POSITION_TEXTURE]);
	else
		glBindTexture(GL_TEXTURE_2D, m_textures[POSITION_TEXTURE]);
}

void 
GBuffer::bindForWriting() {
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fbo);
	//GLenum DrawBuffers[] = { GL_DEPTH_ATTACHMENT, GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1/*, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 */}; 
  //glDrawBuffers(NUM_TEXTURES, DrawBuffers);
}