cmake_minimum_required(VERSION 3.23 FATAL_ERROR)

set(PROJECT_EDITOR_NAME TestProjectEditor)

add_executable(${PROJECT_EDITOR_NAME} 
	src/Main.cpp
)

target_link_libraries(${PROJECT_EDITOR_NAME} TestProjectCore)
target_compile_features(${PROJECT_EDITOR_NAME} PUBLIC cxx_std_17)

set_target_properties(${PROJECT_EDITOR_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/)

#add_custom_command(TARGET ${PROJECT_EDITOR_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory	${CMAKE_SOURCE_DIR}/res $<TARGET_FILE_DIR:${PROJECT_EDITOR_NAME}>)