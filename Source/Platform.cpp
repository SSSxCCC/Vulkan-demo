#include "Platform.h"
#include <fstream>
#include <iostream>

void Platform::createSurface(VkInstance& instance, VkSurfaceKHR& surface) {
#if defined(_WIN32)
    if (glfwCreateWindowSurface(instance, mWindow, nullptr, &surface) != VK_SUCCESS) {
        throw std::runtime_error("failed to create window surface!");
    }
#elif defined(__ANDROID__)
    VkAndroidSurfaceCreateInfoKHR createInfo{VK_STRUCTURE_TYPE_ANDROID_SURFACE_CREATE_INFO_KHR};
    createInfo.window = mApp->window;
    if (vkCreateAndroidSurfaceKHR(instance, &createInfo, nullptr, &surface) != VK_SUCCESS) {
        throw std::runtime_error("failed to create window surface!");
    }
#endif
}

void Platform::getSurfaceSize(int& width, int& height) {
#if defined(_WIN32)
    glfwGetFramebufferSize(mWindow, &width, &height);
#elif defined(__ANDROID__)
    width = ANativeWindow_getWidth(mApp->window);
    height = ANativeWindow_getHeight(mApp->window);
#endif
}

void Platform::waitSurfaceSize() {
#if defined(_WIN32)
    int width = 0, height = 0;
    getSurfaceSize(width, height);
    while (width == 0 || height == 0) {
        getSurfaceSize(width, height);
        glfwWaitEvents();
    }
#elif defined(__ANDROID__)
#endif
}

std::vector<char> Platform::readFile(const std::string& filename) {
#if defined(_WIN32)
    std::ifstream file(filename, std::ios::ate | std::ios::binary);

    if (!file.is_open()) {
        throw std::runtime_error("failed to open file!");
    }

    size_t fileSize = (size_t)file.tellg();
    std::vector<char> buffer(fileSize);
    file.seekg(0);
    file.read(buffer.data(), fileSize);
    file.close();

    return buffer;
#elif defined(__ANDROID__)
    AAsset* file = AAssetManager_open(mApp->activity->assetManager, filename.c_str(), AASSET_MODE_BUFFER);

    if (!file) {
        throw std::runtime_error("failed to open file!");
    }

    size_t fileSize = AAsset_getLength(file);
    std::vector<char> buffer(fileSize);
    AAsset_read(file, buffer.data(), fileSize);
    AAsset_close(file);

    return buffer;
#endif
}

std::vector<const char*> Platform::getExtensions() {
#if defined(_WIN32)
    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions;
    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
    std::vector<const char*> extensions;
    for (int i = 0; i < glfwExtensionCount; i++) {
        extensions.push_back(glfwExtensions[i]);
    }
    return extensions;
#elif defined(__ANDROID__)
    std::vector<const char*> extensions;
    extensions.push_back("VK_KHR_surface");
    extensions.push_back("VK_KHR_android_surface");
    return extensions;
#endif
}
