/**/
#include "../common/platform.h"
#include "../core/application.h"
#include "shadersource.h"
#include "shaderprogram.h"
#include <fstream>
#include <list>
#include <stdio.h>

ShaderSource::ShaderSource()
	{}
ShaderSource::~ShaderSource() {}
		
void 
ShaderSource::addSource( ShaderType t, const std::string& name )  {
	Source s;
	s.name =name;
	s.type =t;
	s.handle =0;
	m_sourcelist.push_back( s );
}
		
ShaderProgram*
ShaderSource::createProgram( bool noCache ) {
	ShaderProgram *program =0;
	bool error =false;
	
	/* create an unique identifier for this permutation */
	std::string permutation =createIdentifier( m_sourcelist );
	
	/* Try the program cache first */
	if( !noCache ) {
		ShaderProgram *prog =ShaderProgramCache::get( permutation );
		if( prog ) {
			printf( "Reusing cached program with identifier: \n\t`%s'\n", permutation.c_str() );
			return prog;
		}
	}
	
	/* Create a program handle */
	GLuint handle =glCreateProgram();
	
	/* Compile and attach all the sources first */
	for( SourceList::iterator it =m_sourcelist.begin(); it != m_sourcelist.end(); it++ ) {
		Source& s = (*it);
		if( !loadShader( pathTo( s.name, s.type ), s.handle, (GLenum)s.type ) ) {
			// error message set
			fprintf( stderr, "%s\n", m_errormsg.c_str() );
			error =true;
			break;
		}
		glAttachShader( handle, s.handle );
	}
	
	/* Link the program and catch possible errors */
	
	if( !error ) {
		glLinkProgram( handle );
	
		GLint result, size = 0;
		const size_t infosize = 2048;
		GLchar info[infosize];
	
		glGetProgramiv( handle , GL_LINK_STATUS, &result );
		if (!result)
		{
			glGetProgramInfoLog( handle, infosize, &size, info );
			m_errormsg = "Shader linking error (" + permutation + "): " + std::string(info, size);
			error =true;
		}
	}
	
	/* Now delete the sources */
	
	for( SourceList::iterator it =m_sourcelist.begin(); it != m_sourcelist.end(); it++ ) {
		Source& s = (*it);
		if( s.handle ) {
			glDetachShader( handle, s.handle );
			glDeleteShader( s.handle );
			s.handle =0;
		}
	}
	
	printf( "Linked new program with identifier: \n\t`%s'\n", permutation.c_str() );

	if( !error ) {
		program =new ShaderProgram();
		program->handle =handle;
		program->ready =true;
		program->permutation =permutation;
		
		/* Append to the cache as well */
		if( !noCache )
			ShaderProgramCache::append( program );
	}
	return program;
}

std::string 
ShaderSource::pathTo( const std::string& shader, ShaderType t ) {
	return Application::app()->resourceDir() + SHADER_DIR_NAME + PATH_SEPARATOR + shader + postfix( t );
}

std::string 
ShaderSource::postfix( ShaderType t ) {
	switch( t ) {
		case VERTEX_SHADER:
			return "_vs.c";
		case FRAGMENT_SHADER:
			return "_fs.c";
		case GEOMETRY_SHADER:
			return "_gs.c";
		case TESS_CONTROL_SHADER:
			return "_tcs.c";
		case TESS_EVALUATION_SHADER:
			return "_tes.c";
/*		case COMPUTE_SHADER:
			return "_cs.c";	*/
	}
	return "";
}

std::string 
ShaderSource::createIdentifier( const SourceList& list ) {

	std::list<std::string> sources;
	for( SourceList::const_iterator it =list.begin(); it != list.end(); it++ ) {
		sources.push_back( (*it).name + postfix( (*it).type ) );
	}
	sources.sort();
	
	std::string identifier;
	
	for( std::list<std::string>::iterator it =sources.begin(); it != sources.end(); it++ ) {
		identifier += (*it);
		if( (*it) != sources.back() ) 
			identifier += "+";
	}
	
	return identifier;
}

bool 
ShaderSource::readShaderFile(const std::string & filename, std::string & shader)
{
	try {
		std::streamoff size;
		std::ifstream file(filename.c_str(), std::ios::binary);
		if (!file.is_open())
		{
			throw("Unable to open the file.");
		}
		file.seekg(0, std::ios::end);
		size = file.tellg();
		file.seekg(0, std::ios::beg);
		size -= file.tellg();
		shader.resize((size_t)size, '\0');
		file.read(&shader[0], size);
		file.close();
	}
	catch(const char * str)
	{
		m_errormsg = std::string("Failed to read shader file ") + filename + ": " + str;
		return false;
	}
	return true;
}

bool 
ShaderSource::loadShader(const std::string & filename, GLuint & shaderid, GLenum type)
{
	std::string shader;
	GLint result, size = 0;
	const size_t infosize = 2048;
	GLchar info[infosize];


	shaderid = glCreateShader(type);
	if (!readShaderFile(filename, shader))
		return false;
	const GLchar * glshader = shader.c_str();
	glShaderSource(shaderid, 1, &glshader, NULL);
	glCompileShader(shaderid);

	glGetShaderiv(shaderid, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(shaderid, infosize, &size, info);
		m_errormsg = "Shader compiling error (" + filename + "):\n" + shader + "\n" + std::string(info, size);
		return false;
	}

	return true;
}
