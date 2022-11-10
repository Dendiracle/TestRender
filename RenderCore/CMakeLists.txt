cmake_minimum_required(VERSION 3.23)

set(PROJECT_CORE_NAME TestProjectCore)

set(CORE_PUBLIC_INCLUDES
	includes/Core/Utils/WindowEvents.hpp
	includes/Core/Application.hpp
	includes/Core/Camera.hpp
	includes/Core/Utils/Log.hpp
	includes/Core/Utils/Keys.hpp
	includes/Core/Utils/Input.hpp
)

set(CORE_PRIVATE_INCLUDES
	includes/Core/ApplicationWindow/WindowBase.hpp
	includes/Core/ApplicationWindow/WindowOpenGL.hpp
	includes/Core/Modules/UIModule.hpp
	includes/Core/Resources/ResourceManager.hpp
	includes/Core/Rendering/OpenGL/Renderer.hpp
	includes/Core/Rendering/OpenGL/ShaderProgram.hpp
	includes/Core/Rendering/OpenGL/VertexBuffer.hpp
	includes/Core/Rendering/OpenGL/VertexArray.hpp
	includes/Core/Rendering/OpenGL/IndexBuffer.hpp
	includes/Core/Rendering/OpenGL/Texture2D.hpp
	includes/Core/Utils/stb_image.h
)

set(CORE_PRIVATE_SOURCES
	src/Core/Application.cpp
	src/Core/Camera.cpp
	src/Core/Utils/Input.cpp
	src/Core/ApplicationWindow/WindowBase.cpp
	src/Core/ApplicationWindow/WindowOpenGL.cpp
	src/Core/Modules/UIModule.cpp
	src/Core/Resources/ResourceManager.cpp
	src/Core/Rendering/OpenGL/Renderer.cpp
	src/Core/Rendering/OpenGL/ShaderProgram.cpp
	src/Core/Rendering/OpenGL/VertexBuffer.cpp
	src/Core/Rendering/OpenGL/VertexArray.cpp
	src/Core/Rendering/OpenGL/IndexBuffer.cpp
	src/Core/Rendering/OpenGL/Texture2D.cpp
)

set(CORE_ALL_SOURCES
	${CORE_PUBLIC_INCLUDES}
	${CORE_PRIVATE_INCLUDES}
	${CORE_PRIVATE_SOURCES}
)

add_library(${PROJECT_CORE_NAME} STATIC
	${CORE_ALL_SOURCES}
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES
	${CORE_ALL_SOURCES}
)

target_include_directories(${PROJECT_CORE_NAME} PUBLIC includes)
#target_include_directories(${PROJECT_CORE_NAME} PRIVATE src)
target_compile_features(${PROJECT_CORE_NAME} PUBLIC cxx_std_17)

add_subdirectory(../external/glfw ${CMAKE_CURRENT_BINARY_DIR}/glfw)
target_link_libraries(${PROJECT_CORE_NAME} PRIVATE glfw)

add_subdirectory(../external/spdlog ${CMAKE_CURRENT_BINARY_DIR}/spdlog)
target_link_libraries(${PROJECT_CORE_NAME} PRIVATE spdlog)

add_subdirectory(../external/glad ${CMAKE_CURRENT_BINARY_DIR}/glad)
target_link_libraries(${PROJECT_CORE_NAME} PRIVATE glad)

add_subdirectory(../external/glm ${CMAKE_CURRENT_BINARY_DIR}/glm)
target_link_libraries(${PROJECT_CORE_NAME} PRIVATE glm)

include_directories(../external/rapidjson/include)