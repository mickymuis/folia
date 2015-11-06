#ifndef SHADER_H
#define SHADER_H

#include <map>
#include <string>

class Shader {
	public:
		enum Type {
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
		Shader();
		Shader( const string& name, Type );
		~Shader();
		
		bool load( const string& name, Type );
		void unload();
		
		GLuint handle() const { return m_handle; }
		const std::string& name() const { return m_name; }
		Type type() const { return m_type; }
		GLenum glType() const { return (GLenum)m_type; }
		
		static std::string pathTo( const string& shader, Type );
		static std::string postfix( Type );
		
		const std::string& errorMessage() const { return m_errormsg; }
		bool hasError() const { return m_error; }
		
		std::string uniqueIdentifier() { return m_permutation; }
		
	private:
		std::string m_name;
		std::string m_errormsg;
		bool m_error;
		bool m_loaded;
		GLuint m_handle;
		Type m_type;
		std::string m_permutation;
		
		bool readShaderFile(const std::string & filename, std::string & shader);
		bool loadShader(const std::string & filename, GLuint & shaderid, GLenum type);
	
};

class ShaderCache {
	public:
		static void initialize();
		static void cleanup();
		
		static Shader* load( const string& name, Type );
		

	private:
		ShaderCache();
		~ShaderCache();
		static ShaderCache* m_instance;
		
		map<string, Shader*> m_shaders;
};

#endif