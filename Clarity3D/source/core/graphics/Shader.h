#pragma once
#include <core/Definitions.h>
#include <gl_core_4_3.hpp>
#include <glm/glm.hpp>


class Shader {
public:
	Shader();
	Shader(const std::filesystem::path& vFile, const std::filesystem::path& fFile);

	GLuint id;
	void bind() const;

	void loadFromFile(const std::filesystem::path& file, GLenum shaderType);
	void compile();

	template<class T>
	void setUniform(const std::string& name, const T& value);

private:
	
};
