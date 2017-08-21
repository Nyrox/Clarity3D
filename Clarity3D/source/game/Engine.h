#pragma once
#include <core/Window.h>
#include <core/graphics/Mesh.h>
#include <core/graphics/Shader.h>
#include <core/Camera.h>
#include <game/StaticMesh.h>
#include <game/MarchingCubes.h>
#include <core/physics/BasicCollision.h>

using namespace basic_collision;

struct Node {
	Node() {
		color = glm::vec3((rand() % 100) / 100.f, (rand() % 100) / 100.f, (rand() % 100) / 100.f);
		memset(children, 0, sizeof(Node*) * 8);
	}
	// C++ makes absolutely no coherent sense to me, but apparently this ends up being zeroed out for us, which is nice
	Node* children[8];
	glm::vec3 center;
	float radius;

	glm::vec3 color;

	AABB getAABB() {
		return AABB(center - glm::vec3(radius), glm::vec3(radius * 2.f));
	}

	bool isLeaf() {
		for (auto& it : children) {
			if (it != nullptr) return false;
		}

		return true;
	}
};

struct Terrain {
	Terrain() {
		cube.loadFromFile("assets/cube.ply");
		initialNode.radius = 1;
	}

	std::pair<Node*, float> intersectRay(Ray& ray) {
		return intersectNode(initialNode, ray);
	}

	std::pair<Node*, float> intersectNode(Node& node, Ray& ray) {	
		if (node.isLeaf()) {
			auto result = intersect_ray_aabb(ray, node.getAABB());
			if (result.first == true) {
				return std::make_pair(&node, result.second);
			}
		}
		else {
			std::pair<Node*, float> closest = { nullptr, std::numeric_limits<float>::max() };

			for (auto& it : node.children) {
				auto result = intersectNode(*it, ray);
				if (result.second < closest.second) closest = result;
			}

			return closest;
		}

		return std::make_pair(nullptr, std::numeric_limits<float>::max());
	}

	void draw(Shader& shader) {
		drawNode(initialNode, shader);
	}

	void drawNode(Node& node, Shader& shader) {
		Transform nodeTransform;
		nodeTransform.position = node.center;
		nodeTransform.scale = glm::vec3 { node.radius };
		
		bool hasChildren = false;
		for (auto& it : node.children) {
			if (it == nullptr) continue;
			hasChildren = true;
			drawNode(*it, shader);
		}

		if (!hasChildren) {
			shader.setUniform<glm::mat4>("model", nodeTransform.getModelMatrix());
			shader.setUniform<glm::vec3>("fillColor", node.color);
			cube.draw();
		}
	}

	void split(Node& node) {
		glm::vec3 boundsOffsetTable[8] = {
			{ -0.5, -0.5, -0.5 },
			{ +0.5, -0.5, -0.5 },
			{ -0.5, +0.5, -0.5 },
			{ +0.5, +0.5, -0.5 },
			{ -0.5, -0.5, +0.5 },
			{ +0.5, -0.5, +0.5 },
			{ -0.5, +0.5, +0.5 },
			{ +0.5, +0.5, +0.5 }
		};

		for (int i = 0; i < 8; i++) {
			node.children[i] = new Node();
			node.children[i]->radius = node.radius / 2.f;
			node.children[i]->center = node.center + boundsOffsetTable[i] * node.radius;
		}
	}

	Node initialNode;
	Mesh cube;
	Transform transform;

};

class Engine {
public:
	Engine();

	void start();

	void key_callback(int, int, int, int);

	Window window;
	Shader shader;
	Camera camera;
	std::vector<StaticMesh> staticMeshes;
	Terrain terrain;

};