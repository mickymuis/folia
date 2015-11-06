#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H

#include "shader.h"
#include <string>


class ShaderProgram {
	public:	
		ShaderProgram();
		~ShaderProgram();
		
		void attachShader( Shader* );
		
		bool link();
		
		GLuint handle() const { return m_handle; }

		const std::string& errorMessage() const { return m_errormsg; }
		bool hasError() const { return m_error; }

	private:

		GLuint m_handle;
		std::string m_errormsg;
		bool m_error;

		
};

#endif