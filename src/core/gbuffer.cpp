#include "gbuffer.h"
#include <stdio.h>

GBuffer::GBuffer() : m_fbo(0), m_width(0), m_height(0) {}

GBuffer::~GBuffer() { destroy(); }

bool 
GBuffer::initialize( const Viewport& vp ) {

	int width =vp.width - vp.x;
	int height =vp.height - vp.y;

	/* First time initialize? */
	if( !m_fbo ) {
		glGenFramebuffers(1, &m_fbo); 
	}
	else {
		glDeleteTextures( NUM_TEXTURES, m_textures);
	}
	
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fbo);
	glGenTextures( NUM_TEXTURES, m_textures );
	
	/* Depth */
	glBindTexture(GL_TEXTURE_2D, m_textures[DEPTH_TEXTURE]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT,
								NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_textures[DEPTH_TEXTURE], 0);
	
	/* Diffuse + spec */
	glBindTexture(GL_TEXTURE_2D, m_textures[DIFFUSE_SPEC_TEXTURE]);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_textures[DIFFUSE_SPEC_TEXTURE], 0);
	
	/* Normals */
	glBindTexture(GL_TEXTURE_2D, m_textures[NORMAL_TEXTURE]);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, width, height, 0, GL_RGB, GL_FLOAT, NULL);
 	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_textures[NORMAL_TEXTURE], 0);
	
	GLenum DrawBuffers[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1/*, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 */}; 
  glDrawBuffers(NUM_TEXTURES-1, DrawBuffers);

	GLenum Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

    if (Status != GL_FRAMEBUFFER_COMPLETE) {
        printf("FB error, status: 0x%x\n", Status);
        return false;
    }
    
  m_width =width;
  m_height =height;
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
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_textures[DEPTH_TEXTURE]);
	
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_textures[DIFFUSE_SPEC_TEXTURE]);
	
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, m_textures[NORMAL_TEXTURE]);
}

void 
GBuffer::bindForWriting() {
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fbo);
}