#include "MeshLoaders.h"
#include <fstream>
#include <array>

glm::vec3 calculateTangent(const Vertex& vx, const Vertex& vy, const Vertex& vz) {
	// Calculate edges
	glm::vec3 edge1 = vy.position - vx.position;
	glm::vec3 edge2 = vz.position - vx.position;

	// Calculate delta UV
	glm::vec2 deltaUV1 = vy.uv - vx.uv;
	glm::vec2 deltaUV2 = vz.uv - vx.uv;

	// Some pythagoras shit dunno
	float f = 1.f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);
	glm::vec3 tangent;
	tangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
	tangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
	tangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
	tangent = glm::normalize(tangent);

	return tangent;
}

std::vector<std::string> string_split(const std::string& haystack, char delim) {
	std::vector<std::string> tokens;

	uint32_t prev = 0, pos = 0;
	do {
		pos = haystack.find(delim, prev);
		if (pos == std::string::npos) pos = haystack.length();
		std::string token = haystack.substr(prev, pos - prev);
		if (!token.empty()) tokens.push_back(token);
		prev = pos + 1;
	} while (pos < haystack.length() && prev < haystack.length());

	return tokens;
}

template<class T>
T string_parse_value(std::string value);

template<>
float string_parse_value<float>(std::string value) {
	return std::stof(value);
}

template<>
uint32 string_parse_value<uint32>(std::string value) {
	return static_cast<uint32>(std::stoi(value));
}

template<class T>
std::vector<T> string_parse_vector(std::vector<std::string> data) {
	std::vector<T> values;
	values.reserve(data.size());
	for (auto& it : data) {
		values.push_back(string_parse_value<T>(it));
	}
	return values;
}

std::pair<std::vector<Vertex>, std::vector<uint32>> MeshLoaders::ply(const std::filesystem::path& path) {
	std::string line;
	std::ifstream file(path);
	if (!file.is_open()) {
		std::cerr << "Failed to open file: " << path << "\n";
		throw std::runtime_error("Faild to open file: "s + path.string());
	}

	std::vector<Vertex> vertices;
	std::vector<uint32> indices;

	// Parse the ply header
	while (std::getline(file, line)) {
		auto tokens = string_split(line, ' ');

		if (tokens[0] == "element") {
			if (tokens[1] == "vertex") vertices.reserve(std::stoi(tokens[2]));
		}

		if (tokens[0] == "end_header") {
			break;
		}
	}

	// Vertices
	uint32 vertexIndex = 0;
	while (vertexIndex < vertices.capacity() && std::getline(file, line)) {
		auto tokens = string_split(line, ' ');

		auto values = string_parse_vector<float>(tokens);

		vertices.emplace_back(
			glm::vec3(values[0], values[1], values[2]),
			glm::vec3(values[3], values[4], values[5]),
			glm::vec2(values[6], values[7])
		);
		vertexIndex++;
	}

	// Faces
	while (std::getline(file, line)) {
		auto tokens = string_split(line, ' ');

		auto values = string_parse_vector<uint32>(tokens);

		switch (values[0]) {
		case 3:
		{
			/* Calculate the tangents now that we have all the data we need */
			glm::uvec3 face(values[1], values[2], values[3]);
			Vertex& vx = vertices.at(face.x);
			Vertex& vy = vertices.at(face.y);
			Vertex& vz = vertices.at(face.z);

			glm::vec3 tangent = calculateTangent(vx, vy, vz);

			vx.tangent = tangent;
			vy.tangent = tangent;
			vz.tangent = tangent;

			indices.insert(indices.end(), { face.x, face.y, face.z });
			break;
		}
		case 4:
			std::array<uint32_t, 6> face = { values[1], values[2], values[3], values[1], values[3], values[4] };
			std::array<Vertex*, 6> vs = { &vertices[face[0]], &vertices[face[1]], &vertices[face[2]], &vertices[face[3]], &vertices[face[4]], &vertices[face[5]] };

			// calculate tangents for face 1
			glm::vec3 tangent = calculateTangent(*vs[0], *vs[1], *vs[2]);
			for (int i = 0; i < 3; i++) vs[i]->tangent = tangent;

			// calculate tangents for face 2
			tangent = calculateTangent(*vs[3], *vs[4], *vs[5]);
			for (int i = 3; i < 6; i++) vs[i]->tangent = tangent;

			// add indices
			for (auto& it : face)
				indices.insert(indices.end(), it);
			break;
		}		
	}

	return { vertices, indices };
}