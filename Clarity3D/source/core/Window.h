#pragma once
#include <core/Definitions.h>
#include <gl_core_4_3.hpp>
#include <GLFW/glfw3.h>

struct ContextSettings {
	int32 debugContext = true;
	int32 glVersionMajor = 3;
	int32 glVersionMinor = 2;
	
};


class Window {
public:
	Window(uint32 width, uint32 height, std::string title, ContextSettings& settings = ContextSettings());
	~Window();

	bool isOpen() const;
	void close() const;
	void display() const;
	void pollEvents() const;

	GLFWwindow* handle;
};