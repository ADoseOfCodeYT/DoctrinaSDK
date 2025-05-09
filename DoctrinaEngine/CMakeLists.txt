
#  _____   ____   _____ _______ _____  _____ _   _            ______ _   _  _____ _____ _   _ ______ 
# |  __ \ / __ \ / ____|__   __|  __ \|_   _| \ | |   /\     |  ____| \ | |/ ____|_   _| \ | |  ____|
# | |  | | |  | | |       | |  | |__) | | | |  \| |  /  \    | |__  |  \| | |  __  | | |  \| | |__   
# | |  | | |  | | |       | |  |  _  /  | | | . ` | / /\ \   |  __| | . ` | | |_ | | | | . ` |  __|  
# | |__| | |__| | |____   | |  | | \ \ _| |_| |\  |/ ____ \  | |____| |\  | |__| |_| |_| |\  | |____ 
# |_____/ \____/ \_____|  |_|  |_|  \_\_____|_| \_/_/    \_\ |______|_| \_|\_____|_____|_| \_|______|                                                                                                   
                         
cmake_minimum_required (VERSION 4.0.2)

project ("DoctrinaEngine")

add_library(${PROJECT_NAME} STATIC "DoctrinaEngine.h" 
"Source/Application/Private/deApplication.cpp" "Source/Application/Public/deApplication.h" 
"Source/Core/Private/deCVar.cpp" "Source/Core/Public/deCVar.h" 
"Source/Core/Public/deVersion.h" 
"Source/Renderer/Private/deRenderer.cpp" "Source/Renderer/Public/deRenderer.h"
"Source/Renderer/Private/deRHI_D3D11.cpp" "Source/Renderer/Private/deRHI_D3D11.h"
"Source/Tools/Private/deConsole.cpp" "Source/Tools/Public/deConsole.h")


target_link_libraries(${PROJECT_NAME} PUBLIC SDL3-static)

target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/Source/")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)
endif()
               