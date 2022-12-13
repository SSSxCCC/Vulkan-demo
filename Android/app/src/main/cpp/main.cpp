#include <jni.h>

#include "AndroidOut.h"
//#include "Renderer.h"
#include "VulkanApp.h"

#include <game-activity/GameActivity.cpp>
#include <game-text-input/gametextinput.cpp>

extern "C" {

#include <game-activity/native_app_glue/android_native_app_glue.c>

/*!
 * Handles commands sent to this Android application
 * @param pApp the app the commands are coming from
 * @param cmd the command to handle
 */
void handle_cmd(android_app *pApp, int32_t cmd) {
    switch (cmd) {
        case APP_CMD_INIT_WINDOW: {
            // A new window is created, associate a renderer with it. You may replace this with a
            // "game" class if that suits your needs. Remember to change all instances of userData
            // if you change the class here as a reinterpret_cast is dangerous this in the
            // android_main function and the APP_CMD_TERM_WINDOW handler case.
            pApp->userData = new VulkanApp(std::make_shared<Platform>(pApp));
            break;
        }
        case APP_CMD_WINDOW_RESIZED: {
            auto *vulkanApp = reinterpret_cast<VulkanApp*>(pApp->userData);
            vulkanApp->framebufferResized();
            break;
        }
        case APP_CMD_TERM_WINDOW: {
            // The window is being destroyed. Use this to clean up your userData to avoid leaking
            // resources.
            //
            // We have to check if userData is assigned just in case this comes in really quickly
            if (pApp->userData) {
                auto *vulkanApp = reinterpret_cast<VulkanApp*>(pApp->userData);
                pApp->userData = nullptr;
                delete vulkanApp;
            }
            break;
        }
        default: break;
    }
}

void handleInput(struct android_app *pApp) {
    // handle all queued inputs
    for (auto i = 0; i < pApp->motionEventsCount; i++) {

        // cache the current event
        auto &motionEvent = pApp->motionEvents[i];

        // cache the current action
        auto action = motionEvent.action;

        // Find the pointer index, mask and bitshift to turn it into a readable value
        auto pointerIndex = (action & AMOTION_EVENT_ACTION_POINTER_INDEX_MASK)
                >> AMOTION_EVENT_ACTION_POINTER_INDEX_SHIFT;
        aout << "Pointer " << pointerIndex << ":";

        // get the x and y position of this event
        auto &pointer = motionEvent.pointers[pointerIndex];
        auto x = GameActivityPointerAxes_getX(&pointer);
        auto y = GameActivityPointerAxes_getY(&pointer);
        aout << "(" << x << ", " << y << ") ";

        // Only consider touchscreen events, like touches
        auto actionMasked = action & AINPUT_SOURCE_TOUCHSCREEN;

        // determine the kind of event it is
        switch (actionMasked) {
            case AMOTION_EVENT_ACTION_DOWN:
            case AMOTION_EVENT_ACTION_POINTER_DOWN:
                aout << "Pointer Down";
                break;

            case AMOTION_EVENT_ACTION_UP:
            case AMOTION_EVENT_ACTION_POINTER_UP:
                aout << "Pointer Up";
                break;

            default:
                aout << "Pointer Move";
        }

        aout << std::endl;
    }

    // clear inputs, be careful as this will clear it for anyone listening to these events
    android_app_clear_motion_events(pApp);
}

/*!
 * This the main entry point for a native activity
 */
void android_main(struct android_app *pApp) {
    // Can be removed, useful to ensure your code is running
    aout << "Welcome to android_main" << std::endl;

    // register an event handler for Android events
    pApp->onAppCmd = handle_cmd;

    // This sets up a typical game/event loop. It will run until the app is destroyed.
    int events;
    android_poll_source *pSource;
    do {
        // Process all pending events before running game logic.
        if (ALooper_pollAll(0, nullptr, &events, (void **) &pSource) >= 0) {
            if (pSource) {
                pSource->process(pApp, pSource);
            }
        }

        // Check if any user data is associated. This is assigned in handle_cmd
        if (pApp->userData) {

            // We know that our user data is a Renderer, so reinterpret cast it. If you change your
            // user data remember to change it here
            auto *vulkanApp = reinterpret_cast<VulkanApp*>(pApp->userData);

            // Process game input
            handleInput(pApp);

            // Render a frame
            vulkanApp->drawFrame();
        }
    } while (!pApp->destroyRequested);
}
}
