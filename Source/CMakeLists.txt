cmake_minimum_required (VERSION 3.12)

add_executable (VulkanDemo "main.cpp")
target_include_directories (VulkanDemo PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

set_property (TARGET VulkanDemo PROPERTY CXX_STANDARD 20)

target_link_libraries (VulkanDemo glfw)
target_link_libraries (VulkanDemo glm)

find_package(Vulkan REQUIRED)
target_include_directories(VulkanDemo PUBLIC ${Vulkan_INCLUDE_DIRS})
target_link_libraries (VulkanDemo ${Vulkan_LIBRARIES})

install (TARGETS VulkanDemo DESTINATION bin)
