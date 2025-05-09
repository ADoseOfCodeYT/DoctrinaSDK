cmake_minimum_required (VERSION 4.0.2)

project("Editor")


add_executable(${PROJECT_NAME} "Source/main.cpp")


target_link_libraries(${PROJECT_NAME} PRIVATE DoctrinaEngine)