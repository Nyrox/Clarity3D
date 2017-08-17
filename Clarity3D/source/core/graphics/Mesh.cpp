#include "Mesh.h"
#include <core/graphics/mesh_loaders/MeshLoaders.h>

Mesh::Mesh(const std::filesystem::path& path) {
	loadFromFile(path);
}

void Mesh::loadFromFile(const std::filesystem::path& path) {
	
	if (path.extension() == ".ply") {
		auto data = MeshLoaders::ply(path);
		initRenderData(data.first, data.second);
	}
	else {
		throw std::runtime_error("Unrecognized file format.");
	}
}

void Mesh::draw() const {
	if (vao == 0) {
		std::cout << "Error::Mesh::draw: Tried to draw mesh without a valid VAO" << std::endl;
		return;
	}

	gl::BindVertexArray(vao);
	gl::DrawElements(gl::TRIANGLES, indexCount, gl::UNSIGNED_INT, 0);
	gl::BindVertexArray(0);
}

void Mesh::initRenderData(std::vector<Vertex> vertices, std::vector<uint32> indices) {
	vertexCount = vertices.size();
	indexCount = indices.size();

	gl::CreateVertexArrays(1, &this->vao);
	gl::CreateBuffers(1, &vbo);
	gl::CreateBuffers(1, &ebo);

	gl::BindVertexArray(this->vao);

	gl::BindBuffer(gl::ARRAY_BUFFER, vbo);
	gl::NamedBufferData(vbo, sizeof(Vertex) * vertices.size(), vertices.data(), gl::STATIC_DRAW);

	gl::BindBuffer(gl::ELEMENT_ARRAY_BUFFER, ebo);
	gl::NamedBufferData(ebo, sizeof(uint32) * indices.size(), indices.data(), gl::STATIC_DRAW);

	// Positions
	gl::VertexAttribPointer(0, 3, gl::FLOAT, 0, sizeof(Vertex), 0);
	gl::EnableVertexAttribArray(0);
	// Normals
	gl::VertexAttribPointer(1, 3, gl::FLOAT, 0, sizeof(Vertex), (GLvoid*)(offsetof(Vertex, normal)));
	gl::EnableVertexAttribArray(1);
	// Texture Coords
	gl::VertexAttribPointer(2, 2, gl::FLOAT, 0, sizeof(Vertex), (GLvoid*)(offsetof(Vertex, uv)));
	gl::EnableVertexAttribArray(2);
	// Tangents
	gl::VertexAttribPointer(3, 3, gl::FLOAT, 0, sizeof(Vertex), (GLvoid*)(offsetof(Vertex, tangent)));
	gl::EnableVertexAttribArray(3);

	gl::BindVertexArray(0);
}