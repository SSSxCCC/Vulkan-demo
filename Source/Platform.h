#ifndef _Platform_H_
#define _Platform_H_

#include <sstream>

#if defined(_WIN32)
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#elif defined(__ANDROID__)
#define VK_USE_PLATFORM_ANDROID_KHR
#include <android/log.h>
#include <game-activity/native_app_glue/android_native_app_glue.h>

/*!
 * Use this class to create an output stream that writes to logcat. By default, a global one is
 * defined as @a aout
 */
class AndroidOut : public std::stringbuf {
   public:
    /*!
     * Creates a new output stream for logcat
     * @param kLogTag the log tag to output
     */
    inline AndroidOut(const char* kLogTag) : logTag_(kLogTag) {}

   protected:
    virtual int sync() override {
        __android_log_print(ANDROID_LOG_DEBUG, logTag_, "%s", str().c_str());
        str("");
        return 0;
    }

   private:
    const char* logTag_;
};
#endif

#include <vulkan/vulkan.h>
#include <vector>
#include <iostream>

class Platform {
public:
#if defined(_WIN32)
    Platform(GLFWwindow* window) : mWindow(window) { };
    std::ostream& mOut = std::cout;
#elif defined(__ANDROID__)
    Platform(android_app* app) : mApp(app), mAndroidOut("AO"), mOut(&mAndroidOut) { };
    std::ostream mOut;
#endif
    void createSurface(VkInstance& instance, VkSurfaceKHR& surface);
    void getSurfaceSize(int& width, int& height);
    void waitSurfaceSize();
    std::vector<char> readFile(const std::string& filename);
    std::vector<const char*> getExtensions();
private:
#if defined(_WIN32)
    GLFWwindow* mWindow;
#elif defined(__ANDROID__)
    android_app* mApp;
    AndroidOut mAndroidOut;
#endif
};

#endif // _Platform_H_
