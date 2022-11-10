#pragma once

#include <glm/mat4x4.hpp>

namespace rec
{
	class ShaderProgram
	{
	public:
		ShaderProgram(const char* vertexShaderSrc, const char* fragmentShaderSrc);
		ShaderProgram(ShaderProgram&&);
		ShaderProgram& operator=(ShaderProgram&&);
		~ShaderProgram();

		ShaderProgram() = delete;
		ShaderProgram(const ShaderProgram&) = delete;
		ShaderProgram operator=(ShaderProgram&) = delete;

		void bind() const;
		static void unbind();
		bool is_compiled();
		void set_matrix4(const char* name, const glm::mat4& matrix) const;
		void set_int(const char* name, const int value) const;

	private:
		bool bIsCompiled = false;
		unsigned int mID = 0;
	};
}