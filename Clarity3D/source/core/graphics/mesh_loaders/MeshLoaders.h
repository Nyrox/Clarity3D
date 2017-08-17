#pragma once
#include <core/Definitions.h>
#include <core/graphics/Vertex.h>


namespace MeshLoaders {
	std::pair<std::vector<Vertex>, std::vector<uint32>> ply(const std::filesystem::path& path);
}