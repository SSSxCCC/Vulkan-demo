#ifndef _VulkanApp_H_
#define _VulkanApp_H_

#include "Platform.h"
#include <optional>
#include <vector>
#include <cstdint>    // Necessary for uint32_t

class VulkanApp {
public:
    VulkanApp(Platform* platform) : mPlatform(platform) { initVulkan(); }
    ~VulkanApp() { cleanup(); delete mPlatform; }

    void drawFrame();

    void framebufferResized() {
        mFramebufferResized = true;
    }

private:
    Platform* mPlatform;

    VkInstance mInstance;

    const std::vector<const char*> VALIDATION_LAYERS = {
            "VK_LAYER_KHRONOS_validation"
    };
#ifdef NDEBUG
    const bool ENABLE_VALIDATION_LAYERS = false;
#else
    const bool ENABLE_VALIDATION_LAYERS = true;
#endif

    VkPhysicalDevice mPhysicalDevice = VK_NULL_HANDLE;
    VkDevice mDevice;
    VkPhysicalDeviceFeatures mDeviceFeatures{};
    VkQueue mGraphicsQueue;
    VkQueue mPresentQueue;

    VkSurfaceKHR mSurface;

    const std::vector<const char*> DEVICE_EXTENSIONS = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };

    VkSwapchainKHR mSwapChain;
    std::vector<VkImage> mSwapChainImages;
    VkFormat mSwapChainImageFormat;
    VkExtent2D mSwapChainExtent;
    std::vector<VkImageView> mSwapChainImageViews;

    VkRenderPass mRenderPass;
    VkPipelineLayout mPipelineLayout;
    VkPipeline mGraphicsPipeline;

    std::vector<VkFramebuffer> mSwapChainFramebuffers;

    const int MAX_FRAMES_IN_FLIGHT = 2;

    VkCommandPool mCommandPool;
    std::vector<VkCommandBuffer> mCommandBuffers;

    std::vector<VkSemaphore> mImageAvailableSemaphores;
    std::vector<VkSemaphore> mRenderFinishedSemaphores;
    std::vector<VkFence> mInFlightFences;

    uint32_t mCurrentFrame = 0;
    bool mFramebufferResized = false;

    struct QueueFamilyIndices {
        std::optional<uint32_t> graphicsFamily;
        std::optional<uint32_t> presentFamily;

        bool isComplete() {
            return graphicsFamily.has_value() && presentFamily.has_value();
        }
    };

    struct SwapChainSupportDetails {
        VkSurfaceCapabilitiesKHR capabilities;
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR> presentModes;
    };

    void initVulkan();

    void createInstance();

    bool checkValidationLayerSupport();

    void createSurface();

    void pickPhysicalDevice();

    bool isDeviceSuitable(VkPhysicalDevice device);

    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);

    bool checkDeviceExtensionSupport(VkPhysicalDevice device);

    SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);

    void createLogicalDevice();

    void createSwapChain();

    VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);

    VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);

    VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

    void createImageViews();

    void createRenderPass();

    void createGraphicsPipeline();

    VkShaderModule createShaderModule(const std::vector<char>& code);

    void createFramebuffers();

    void createCommandPool();

    void createCommandBuffers();

    void createSyncObjects();

    void recreateSwapChain();

    void cleanupSwapChain();

    void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);

    void cleanup();
};

#endif // _VulkanApp_H_
