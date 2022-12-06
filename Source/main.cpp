#include "Platform.h"
#include "VulkanApp.h"
#include <iostream>

static void framebufferResizeCallback(GLFWwindow* window, int width, int height) {
    auto app = reinterpret_cast<VulkanApp*>(glfwGetWindowUserPointer(window));
    app->framebufferResized();
}

int main() {
    try {
        glfwInit();
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        GLFWwindow* window = glfwCreateWindow(800, 600, "Vulkan", nullptr, nullptr);
        glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
        
        VulkanApp* vulkanApp = new VulkanApp(new Platform(window));
        glfwSetWindowUserPointer(window, vulkanApp);

        while (!glfwWindowShouldClose(window)) {
            glfwPollEvents();
            vulkanApp->drawFrame();
        }

        delete vulkanApp;
        glfwDestroyWindow(window);
        glfwTerminate();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
