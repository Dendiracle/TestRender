cmake_minimum_required(VERSION 3.23)

set(PROJECT_NAME TestProject)

add_subdirectory(RenderCore)
add_subdirectory(RenderEditor)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT RenderCore)