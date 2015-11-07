/**/
#ifndef SHADERSOURCE_H
#define SHADERSOURCE_H

#include "../common/platform.h"

#include <string>
#include <vector>

class ShaderProgram;

enum ShaderType {
	VERTEX_SHADER =
		GL_VERTEX_SHADER,
	FRAGMENT_SHADER =
		GL_FRAGMENT_SHADER,
	GEOMETRY_SHADER =
		GL_GEOMETRY_SHADER,
	TESS_CONTROL_SHADER =
		GL_TESS_CONTROL_SHADER,
	TESS_EVALUATION_SHADER =
		GL_TESS_EVALUATION_SHADER,
	COMPUTE_SHADER =
		GL_COMPUTE_SHADER
};

class ShaderSource {
	public:	
	
		ShaderSource();
		~ShaderSource();
		
		void addSource( ShaderType, const std::string& name );
		
		ShaderProgram* createProgram( bool noCache =false );
		
		const std::string& errorMessage() const { return m_errormsg; }
		
		std::string uniqueIdentifier() const { return createIdentifier( m_sourcelist ); }
		
		static std::string pathTo( const std::string& shader, ShaderType );
		static std::string postfix( ShaderType );

	private:
		class Source {
			public:
				ShaderType type;
				std::string name;
				GLuint handle;
		};
		
		typedef std::vector<Source> SourceList;
		SourceList m_sourcelist;

		std::string m_errormsg;

		static std::string createIdentifier( const SourceList& );
		bool readShaderFile(const std::string & filename, std::string & shader);
		bool loadShader(const std::string & filename, GLuint & shaderid, GLenum type);
};

#endif