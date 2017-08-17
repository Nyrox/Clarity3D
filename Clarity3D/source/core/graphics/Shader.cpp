#include "Shader.h"
#include <fstream>
#include <sstream>

#include <glm/gtc/type_ptr.hpp>

void checkCompileErrors(GLuint object, GLenum type) {
	GLint success;
	GLchar infoLog[1024];

	if (type == gl::PROGRAM) {
		gl::GetProgramiv(object, gl::LINK_STATUS, &success);
		if (!success) {
			gl::GetProgramInfoLog(object, 1024, NULL, infoLog);
		}
	}
	else {
		gl::GetShaderiv(object, gl::COMPILE_STATUS, &success);
		if (!success) {
			gl::GetShaderInfoLog(object, 1024, NULL, infoLog);
		}
	}
	if (!success) {
		std::cout << "ERROR::SHADER: \n" << infoLog << std::endl;
		std::cin.get();
	}
}

Shader::Shader() {
	this->id = gl::CreateProgram();
}

Shader::Shader(const std::filesystem::path& vFile, const std::filesystem::path& fFile) : Shader() {
	loadFromFile(vFile, gl::VERTEX_SHADER);
	loadFromFile(fFile, gl::FRAGMENT_SHADER);
	compile();
}

void Shader::bind() const {
	gl::UseProgram(this->id);
}

void Shader::loadFromFile(const std::filesystem::path& path, GLenum shaderType) {

	std::ifstream file(path);
	if (!file.is_open()) {
		std::cout << "Failed to open file: " << path << "\n";
	}

	std::stringstream ss;
	ss << file.rdbuf();
	file.close();
	auto str = ss.str();
	const char* source = str.c_str();

	std::cout << str << "\n";
	
	GLuint shader = gl::CreateShader(shaderType);
	gl::ShaderSource(shader, 1, &source, NULL);
	gl::CompileShader(shader);
	checkCompileErrors(shader, shaderType);

	gl::AttachShader(this->id, shader);
	//gl::DeleteShader(shader);
}

void Shader::compile() {
	gl::LinkProgram(this->id);
	checkCompileErrors(this->id, gl::PROGRAM);
}



template<>
void Shader::setUniform<glm::mat4>(const std::string& name, const glm::mat4& value) {
	gl::ProgramUniformMatrix4fv(this->id, gl::GetUniformLocation(this->id, name.c_str()), 1, false, glm::value_ptr(value));
}

template<>
void Shader::setUniform<glm::vec3>(const std::string& name, const glm::vec3& value) {
	gl::ProgramUniform3fv(this->id, gl::GetUniformLocation(this->id, name.c_str()), 1, glm::value_ptr(value));
}