#include "shader.h"
#include "../common/platform.h"
#include "../core/application.h"

Shader::Shader() : bool m_error ( false ),
		bool m_loaded ( false ),
		GLuint m_handle ( 0 )
 {}

Shader::Shader( const string& name, Type t ) : bool m_error ( false ),
		bool m_loaded ( false ),
		GLuint m_handle ( 0 )
{
	load( name, t );	
}

Shader::~Shader() {
	unload();
}
		
bool 
Shader::load( const string& name, Type t ) {
	m_type =t;
	m_name =name;
	m_permutation =name + postfix( t );
	
	m_loaded =loadShader( pathTo( name, t ), m_handle, glType() );
	m_error =!m_loaded;
	
	return !m_error;
}

void 
Shader::unload() {

}
		
std::string 
Shader::pathTo( const string& shader, Type t ) {
	return Application::app()->resourceDir() + SHADER_DIR_NAME + PATH_SEPARATOR + shader + postfix( t );
}

std::string 
Shader::postfix( Type t ) {
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
		case COMPUTE_SHADER:
			return "_cs.c";	
	}
	return "";
}

bool 
Shader::readShaderFile(const std::string & filename, std::string & shader)
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
		errormsg = std::string("Failed to read shader file ") + filename + ": " + str;
		return false;
	}
	return true;
}

bool 
Shader::loadShader(const std::string & filename, GLuint & shaderid, GLenum type)
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
		errormsg = "Shader compiling error (" + filename + "):\n" + shader + "\n" + std::string(info, size);
		return false;
	}

	return true;
}