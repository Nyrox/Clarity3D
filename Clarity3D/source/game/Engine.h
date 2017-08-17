#pragma once
#include <core/Window.h>
#include <core/graphics/Mesh.h>
#include <core/graphics/Shader.h>
#include <core/Camera.h>
#include <game/StaticMesh.h>
#include <game/MarchingCubes.h>

struct Node {
	// C++ makes absolutely no coherent sense to me, but apparently this ends up being zeroed out for us, which is nice
	Node* children[8];
	glm::vec3 center;
	float radius;
};

struct Terrain {
	Terrain() {
		cube.loadFromFile("assets/cube.ply");
		initialNode.radius = 1;

		split(initialNode);
		split(*initialNode.children[0]);
		split(*initialNode.children[0]->children[4]);
		split(*initialNode.children[4]);
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
			shader.setUniform<glm::vec3>("fillColor", glm::vec3((rand() % 100) / 100.f, (rand() % 100) / 100.f, (rand() % 100) / 100.f));
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
			node.children[i]->radius = node.radius / 2;
			node.children[i]->center = node.center + boundsOffsetTable[i] * node.radius / 2.f;
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

	Window window;
	Shader shader;
	Camera camera;
	std::vector<StaticMesh> staticMeshes;
	Terrain terrain;

};