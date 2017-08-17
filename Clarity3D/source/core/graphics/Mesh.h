#pragma once
#include <core/Definitions.h>
#include <gl_core_4_3.hpp>
#include <core/graphics/Vertex.h>

class Mesh {
public:
	Mesh() = default;
	Mesh(const std::filesystem::path& path);

	void loadFromFile(const std::filesystem::path& path);
	void initRenderData(std::vector<Vertex> vertices, std::vector<uint32> indices);
	void draw() const;

	uint32 vertexCount, indexCount;
	GLuint vao, vbo, ebo;
};