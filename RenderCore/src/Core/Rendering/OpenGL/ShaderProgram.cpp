#include "Core/Rendering/OpenGL/ShaderProgram.hpp"

#include "Core/Utils/Log.hpp"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>


namespace rec
{
	bool createShader(const char* source, const GLenum shaderType, GLuint& shaderID)
	{
		shaderID = glCreateShader(shaderType);
		glShaderSource(shaderID, 1, &source, nullptr);
		glCompileShader(shaderID);

		GLint succes;
		glGetShaderiv(shaderID, GL_COMPILE_STATUS, &succes);

		if (succes == GL_FALSE)
		{
			char infoLog[1024];
			glGetShaderInfoLog(shaderID, 1024, nullptr, infoLog);

			logCritical("Shader compilation error: \n{}", infoLog);
			return false;
		}

		return true;
	}

	rec::ShaderProgram::ShaderProgram(const char* vertexShaderSrc, const char* fragmentShaderSrc)
	{
		GLuint vertexShaderID = 0;
		if (!createShader(vertexShaderSrc, GL_VERTEX_SHADER, vertexShaderID))
		{
			logCritical("VERTEX SHADER: compile time error.");
			glDeleteShader(vertexShaderID);
			return;
		}

		GLuint fragmentShaderID = 0;
		if (!createShader(fragmentShaderSrc, GL_FRAGMENT_SHADER, fragmentShaderID))
		{
			logCritical("FRAGMENT SHADER: compile time error.");
			glDeleteShader(vertexShaderID);
			glDeleteShader(fragmentShaderID);
			return;
		}

		mID = glCreateProgram();
		glAttachShader(mID, vertexShaderID);
		glAttachShader(mID, fragmentShaderID);
		glLinkProgram(mID);

		GLint succes;
		glGetProgramiv(mID, GL_LINK_STATUS, &succes);
		if (succes == GL_FALSE)
		{
			GLchar info_log[1024];
			glGetProgramInfoLog(mID, 1024, nullptr, info_log);
			logCritical("SHADER PROGRAM: Link-time error:\n{0}", info_log);
			glDeleteProgram(mID);
			mID = 0;
			glDeleteShader(vertexShaderID);
			glDeleteShader(fragmentShaderID);
			return;
		}

		bIsCompiled = true;
		
		glDetachShader(mID, vertexShaderID);
		glDetachShader(mID, fragmentShaderID);
		glDeleteShader(vertexShaderID);
		glDeleteShader(fragmentShaderID);
	}

	rec::ShaderProgram::ShaderProgram(ShaderProgram&& shaderProgram)
	{
		mID = shaderProgram.mID;
		bIsCompiled = shaderProgram.bIsCompiled;

		shaderProgram.mID = 0;
		shaderProgram.bIsCompiled = false;
	}

	ShaderProgram& rec::ShaderProgram::operator=(ShaderProgram&& shaderProgram)
	{
		glDeleteProgram(mID);
		mID = shaderProgram.mID;
		bIsCompiled = shaderProgram.bIsCompiled;

		shaderProgram.mID = 0;
		shaderProgram.bIsCompiled = false;
		return *this;
	}

	rec::ShaderProgram::~ShaderProgram()
	{
		glDeleteProgram(mID);
	}

	void rec::ShaderProgram::bind() const
	{
		glUseProgram(mID);
	}

	void rec::ShaderProgram::unbind()
	{
		glUseProgram(0);
	}

	bool rec::ShaderProgram::is_compiled()
	{
		return this->bIsCompiled;
	}

	void ShaderProgram::set_matrix4(const char* name, const glm::mat4& matrix) const
	{
		glUniformMatrix4fv(glGetUniformLocation(mID, name), 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void ShaderProgram::set_int(const char* name, const int value) const
	{
		glUniform1i(glGetUniformLocation(mID, name), value);
	}

}
