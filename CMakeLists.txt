# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.12)

project ("VulkanDemo" VERSION 0.1)

add_subdirectory ("Dependency/glfw" "glfw")
add_subdirectory ("Dependency/glm" "glm")

add_subdirectory ("Source")

