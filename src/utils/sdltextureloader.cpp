/*
 * This file is part of Folia, an experimental mini-engine using OpenGL 3
 * Created by Micky Faas. Freely usable and modifiable for academic purposes.
 */

#include "sdltextureloader.h"
#include "../core/application.h"
#include <string>
#include <cstdio>

GLuint 
loadTextureBMP( const char* relative_path, bool generateMipmaps ) {
	std::string path = Application::app()->resourceDir() + TEXTURE_DIR_NAME + PATH_SEPARATOR + std::string(relative_path);
	
	SDL_Surface* surface =SDL_LoadBMP( path.c_str() );
	
	if( !surface ) {
		printf("Unable to load image %s! SDL Error: %s\n", relative_path, SDL_GetError());
		return 0;
	}
	
	GLuint tex;
	
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
 
	int mode = GL_RGB;
 
	if(surface->format->BytesPerPixel == 4) {
			mode = GL_RGBA;
	}
 
	glTexImage2D(GL_TEXTURE_2D, 0, mode, surface->w, surface->h, 0, GL_BGR, GL_UNSIGNED_BYTE, surface->pixels);
 	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	
	if( generateMipmaps ) {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
		glGenerateMipmap(GL_TEXTURE_2D);
	} else
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	
	
	
	
	
	printf( "Loaded texture `%s'\n", relative_path );
	
	return tex;
}