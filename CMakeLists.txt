#  __  __           _____ _______ ______ _____  
# |  \/  |   /\    / ____|__   __|  ____|  __ \ 
# | \  / |  /  \  | (___    | |  | |__  | |__) |
# | |\/| | / /\ \  \___ \   | |  |  __| |  _  / 
# | |  | |/ ____ \ ____) |  | |  | |____| | \ \ 
# |_|  |_/_/    \_\_____/   |_|  |______|_|  \_\
                             
# Holy fucking shit CMake makes me want to kill myself
# My fucking c key is so unreliable it just doesnt work sometimes

cmake_minimum_required (VERSION 4.0.2)

project ("DoctrinaSDK" CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE CXX_EXTENSIONS OFF)

include(FetchContent)

# Configure SDL3

set(SDL_STATIC ON CACHE BOOL "Build SDL3 as a static library")
set(SDL_SHARED OFF CACHE BOOL "Do not build SDL3 as a shared library")
set(SDL_OPENGL OFF CACHE BOOL "Do not build with OpenGL")
set(SDL_OPENGLES OFF CACHE BOOL "Do not build with OpenGLES")
set(SDL_RENDER_D3D12 OFF CACHE BOOL "Do not build with Direct3D 12 renderer")
set(SDL_RENDER_D3D11 OFF CACHE BOOL "Do not build with Direct3D 11 renderer")
set(SDL_VULKAN OFF CACHE BOOL "Do not build with Vulkan support")
set(SDL_RENDER_VULKAN OFF CACHE BOOL "Do not build with Vulkan renderer")

FetchContent_Declare(
  sdl3
  GIT_REPOSITORY https://github.com/libsdl-org/SDL
  GIT_TAG release-3.2.12
)

FetchContent_MakeAvailable(sdl3)

add_compile_definitions("RELEASE=$<CONFIG:Release>")
add_compile_definitions("DEBUG=$<CONFIG:Debug>")

add_subdirectory ("DoctrinaEngine")
add_subdirectory("Editor")

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()