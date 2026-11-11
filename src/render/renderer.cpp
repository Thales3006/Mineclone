#include "render/renderer.h"

#include "render/renderLib.h"

#include <iostream>

Renderer::Renderer(Window &window)
    : window(window), viewportSize(window.getWindowSize()) {
    initOpenGL();
}

void Renderer::initOpenGL() {
    if (!gladLoadGL()) {
        std::cout << "Failed to initialize Glad.\n";
        return;
    }

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glDepthFunc(GL_LESS);
}

void Renderer::beginFrame() {
    glClearColor(0.4f, 0.6f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    viewportSize = window.getWindowSize();
    glViewport(0, 0, viewportSize.x, viewportSize.y);
}

glm::uvec2 Renderer::getViewportSize() { return viewportSize; }
