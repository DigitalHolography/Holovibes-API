cmake_minimum_required(VERSION 3.24)

project(Holovibes
	VERSION 14.4.2
	DESCRIPTION "Holovibes"
	LANGUAGES CXX CUDA
)

# Export compile_commands.json so IDE can use it for code completion.
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

set(LIB_DIR "${CMAKE_SOURCE_DIR}/libs")
set(HOLO_DIR "${CMAKE_SOURCE_DIR}/Holovibes")
set(CAMERA_DIR "${CMAKE_SOURCE_DIR}/Camera")
set(PRESET_DIR "${CMAKE_SOURCE_DIR}/Preset")

# All packages used by the project.
find_package(Boost COMPONENTS program_options REQUIRED)
find_package(OpenCV REQUIRED)
find_package(CUDA 12.2 REQUIRED)
find_package(CUDAToolkit 12.2 REQUIRED)
find_package(spdlog REQUIRED)
find_package(GTest REQUIRED)
find_package(nlohmann_json REQUIRED)
find_package(Threads)

add_subdirectory(Camera)
add_subdirectory(Holovibes)
add_subdirectory(docs)
