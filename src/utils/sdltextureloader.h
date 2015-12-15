/*
 * This file is part of Folia, an experimental mini-engine using OpenGL 3
 * Created by Micky Faas. Freely usable and modifiable for academic purposes.
 */

#ifndef SDLTEXTURELOADER_H
#define SDLTEXTURELOADER_H

/* This texture loader is a stub until something decent is in place */

#include "../common/platform.h"

GLuint loadTextureBMP( const char* relative_path, bool generateMipmaps );

#endif