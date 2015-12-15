/*
 * This file is part of Folia, an experimental mini-engine using OpenGL 3
 * Created by Micky Faas. Freely usable and modifiable for academic purposes.
 */

#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H

#include "shadersource.h"
#include <string>
#include <map>

class ShaderProgram {
	public:	
		
		ShaderProgram();
		~ShaderProgram();
		
		void deleteProgram();
		
		GLuint programHandle() const { return handle; }

		bool isReady() const { return ready; }
		
		std::string uniqueIdentifier() const { return permutation; }


	protected:
		friend class ShaderSource;
		
		GLuint handle;
		std::string permutation;
		bool ready;
};

class ShaderProgramCache {
	public:
		static void initialize();
		static void cleanup();
		
		static ShaderProgram* get( const ShaderSource& );
		static ShaderProgram* get( const std::string& );
		static void append( ShaderProgram* );
		

	private:
		ShaderProgramCache();
		~ShaderProgramCache();
		static ShaderProgramCache* instance;
		
		typedef std::map<std::string, ShaderProgram*> CacheMap;
		CacheMap cache;
	
};

#endif