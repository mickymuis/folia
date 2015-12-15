/*
 * This file is part of Folia, an experimental mini-engine using OpenGL 3
 * Created by Micky Faas. Freely usable and modifiable for academic purposes.
 */

#define REQUIRE_GL_MAJOR_VERSION 3
#define REQUIRE_GL_MINOR_VERSION 3

#define PATH_SEPARATOR "/"
#define SHADER_DIR_NAME "shaders"
#define TEXTURE_DIR_NAME "textures"

#define GLEW_STATIC
#define _USE_MATH_DEFINES
 
#define GL_GLEXT_PROTOTYPES
#include <GL/glew.h>

#if defined (__APPLE_CC__)
#include <OpenGL/gl.h>
#include <OpenGL/glext.h>
#else
#include <GL/gl.h>
#include <GL/glext.h>
#endif
#include <SDL2/SDL.h>

#include <cstdlib>
#include <cmath>
#include <cinttypes>
