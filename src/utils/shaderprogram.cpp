/*
 * This file is part of Folia, an experimental mini-engine using OpenGL 3
 * Created by Micky Faas. Freely usable and modifiable for academic purposes.
 */

#include "../common/platform.h"
#include "shaderprogram.h"

/* ShaderProgram implementation */

ShaderProgram::ShaderProgram() {}
ShaderProgram::~ShaderProgram() {
	deleteProgram();
}
		
void 
ShaderProgram::deleteProgram() {
	if( ready ) {
		glDeleteProgram( handle );
		ready =false;
	}
}

/* ShaderProgramCache implementation */

void 
ShaderProgramCache::initialize() {
	if( !instance )
		instance =new ShaderProgramCache;
}

void 
ShaderProgramCache::cleanup() {
	if( instance ) {
		CacheMap::iterator it;
		for( it = instance->cache.begin(); it != instance->cache.end(); it++ ) {
			delete it->second;
		}
		delete instance;
	}
}
		
ShaderProgram* 
ShaderProgramCache::get( const ShaderSource& src ) {
	return get( src.uniqueIdentifier() );
}

ShaderProgram* 
ShaderProgramCache::get( const std::string& permutation ) {
	initialize();
	CacheMap::iterator it;
	it =instance->cache.find( permutation );
	if( it != instance->cache.end() )
		return it->second;
	return 0;
}

void 
ShaderProgramCache::append( ShaderProgram* prog ) {
	initialize();
	instance->cache[prog->uniqueIdentifier()] = prog;
}
		
ShaderProgramCache* ShaderProgramCache::instance =0;
ShaderProgramCache::ShaderProgramCache() {}
ShaderProgramCache::~ShaderProgramCache() {}