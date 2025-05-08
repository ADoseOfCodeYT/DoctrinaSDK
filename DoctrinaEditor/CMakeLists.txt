
#  ______ _____ _____ _______ ____  _____  
# |  ____|  __ \_   _|__   __/ __ \|  __ \ 
# | |__  | |  | || |    | | | |  | | |__) |
# |  __| | |  | || |    | | | |  | |  _  / 
# | |____| |__| || |_   | | | |__| | | \ \ 
# |______|_____/_____|  |_|  \____/|_|  \_\
                                   
cmake_minimum_required (VERSION 4.0.2)

project ("DoctrinaEditor")

file(GLOB SOURCE_FILES CONFIGURE_DEPENDS "Source/*.cpp")

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_precompile_headers(${PROJECT_NAME} PRIVATE "Source/stdafx.h")

target_link_libraries(${PROJECT_NAME} PRIVATE DoctrinaEngine)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)
endif()