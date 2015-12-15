/*
 * This file is part of Folia, an experimental mini-engine using OpenGL 3
 * Created by Micky Faas. Freely usable and modifiable for academic purposes.
 */

#include "window.h"

Window::Window( const char* title ) : title(title) {

	width = WINDOW_DEFAULT_WIDTH;
	height = WINDOW_DEFAULT_HEIGHT;
}

Viewport *
Window::viewport( ViewportPosition p ) {
	return &viewports[p];
}

void 
Window::updateViewportDimensions( ViewportPosition p ) {
	// TODO: change when layout in implemented
	viewport(p)->width =width;
	viewport(p)->height =height;
	viewport(p)->x =0;
	viewport(p)->y =0;
	viewport(p)->aspect = (float)width / (float)height;
}