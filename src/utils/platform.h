#define GLEW_STATIC
#define _USE_MATH_DEFINES
 
#define GL_GLEXT_PROTOTYPES
#include <GL/glew.h>

#if defined (__APPLE_CC__)
#include <OpenGL/gl3.h>
#include <OpenGL/glext.h>
#else
#include <GL/gl.h>       /* assert OpenGL 3.2 core profile available. */
#include <GL/glext.h>
#endif
#include <SDL2/SDL.h>

#include <cstdlib>
#include <cmath>
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <cinttypes>
