#include "Engine.h"

void __key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	Engine* engine = (Engine*)glfwGetWindowUserPointer(window);
	
	engine->key_callback(key, scancode, action, mods);
}

void Engine::key_callback(int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_F && action == GLFW_RELEASE) {
		auto result = terrain.intersectRay(Ray(camera.transform.position, camera.transform.forwards()));
		if (result.first == nullptr) return;
		terrain.split(*result.first);
	}

}

Engine::Engine() :
	window(1280, 720, "Praise kek!"),
	shader("assets/shaders/test.vert", "assets/shaders/test.frag")
{
	srand(time(NULL));

	camera.projection = glm::perspective(glm::radians(60.f), 1280.f / 720.f, 0.1f, 100.0f);
	camera.transform.position.z = -3;
	camera.projection[0][0] *= -1;

	try {
		staticMeshes.emplace_back("assets/monkey.ply");
		staticMeshes.back().transform.position.x += 3;
	}
	catch (std::runtime_error& e) {
		std::cerr << "Error trying to load mesh...\n" << e.what() << "\n";
	}

	glfwSetWindowUserPointer(window.handle, this);
	glfwSetKeyCallback(window.handle, __key_callback);
}

void Engine::start() {

	gl::Enable(gl::DEPTH_TEST);
	gl::DepthFunc(gl::LESS);
	gl::Disable(gl::CULL_FACE);
	gl::Disable(gl::BLEND);

	while (window.isOpen()) {
		window.pollEvents();


		if (glfwGetKey(window.handle, GLFW_KEY_A) == GLFW_PRESS) {
			camera.transform.position += camera.transform.right() * -0.01f;
		}

		if (glfwGetKey(window.handle, GLFW_KEY_D) == GLFW_PRESS) {
			camera.transform.position += camera.transform.right() * 0.01f;
		}

		if (glfwGetKey(window.handle, GLFW_KEY_Q) == GLFW_PRESS) {
			camera.transform.rotateAround(-0.01f, { 0, 1, 0 });
		}

		if (glfwGetKey(window.handle, GLFW_KEY_E) == GLFW_PRESS) {
			camera.transform.rotateAround(0.01f, { 0, 1, 0 });
		}

		if (glfwGetKey(window.handle, GLFW_KEY_W) == GLFW_PRESS) {
			camera.transform.position += camera.transform.forwards() * 0.01f;
		}

		if (glfwGetKey(window.handle, GLFW_KEY_S) == GLFW_PRESS) {
			camera.transform.position += camera.transform.forwards() * -0.01f;
		}

		if (glfwGetKey(window.handle, GLFW_KEY_SPACE) == GLFW_PRESS) {
			camera.transform.position += camera.transform.up() * 0.01f;
		}

		if (glfwGetKey(window.handle, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
			camera.transform.position += camera.transform.up() * -0.01f;
		}
	


		gl::Clear(gl::COLOR_BUFFER_BIT | gl::DEPTH_BUFFER_BIT);

		shader.bind();
		shader.setUniform<glm::mat4>("view", camera.getViewMatrix());
		shader.setUniform<glm::mat4>("projection", camera.projection);

		for (auto& it : staticMeshes) {
			shader.setUniform<glm::mat4>("model", it.transform.getModelMatrix());
			//it.mesh.draw();
		}

		terrain.draw(shader);

		window.display();
	}
}