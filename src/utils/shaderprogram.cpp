#include "../common/platform.h"
#include "shaderprogram.h"
#include <fstream>

bool ShaderProgram::loadShaderProgram(const std::string & name)
{
	GLint result, size = 0;
	const size_t infosize = 2048;
	GLchar info[infosize];

	this->name = name;

	if (!loadShader(name + "_vs.c", vs, GL_VERTEX_SHADER))
		return false;
	if (!loadShader(name + "_fs.c", fs, GL_FRAGMENT_SHADER))
		return false;

	p = glCreateProgram();
	glAttachShader(p, vs);
	glAttachShader(p, fs);
	
	//glBindAttribLocation( p, 0, "in_position" );
	//glBindAttribLocation( p, 1, "in_normal" );
	
	glLinkProgram(p);

	glGetProgramiv(p, GL_LINK_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(p, infosize, &size, info);
		errormsg = "Shader linking error (" + name + "): " + std::string(info, size);
		return false;
	}

	return true;
}


const std::string & ShaderProgram::getErrorMessage() const
{
	return errormsg;
}
