#pragma once
#include <core/graphics/Mesh.h>
#include <core/Transform.h>

struct StaticMesh {
	StaticMesh() = default;
	StaticMesh(const std::filesystem::path& path) : mesh(path) { }

	Mesh mesh;
	Transform transform;
};