cmake_minimum_required(VERSION 3.10)
project(Tetraxis)

set(CMAKE_CXX_STANDARD 20)

file(GLOB SRC_FILES "src/*.cpp")

add_executable(Tetraxis ${SRC_FILES})

target_include_directories(Tetraxis
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(Tetraxis PRIVATE glfw)
find_package(GLEW REQUIRED)
target_link_libraries(Tetraxis PRIVATE GLEW::GLEW)
find_package(glm CONFIG REQUIRED)
target_link_libraries(Tetraxis PRIVATE glm::glm-header-only)

target_compile_definitions(Tetraxis PRIVATE GLM_EXPERIMENTAL)
