
#  __  __           _____ _______ ______ _____  
# |  \/  |   /\    / ____|__   __|  ____|  __ \ 
# | \  / |  /  \  | (___    | |  | |__  | |__) |
# | |\/| | / /\ \  \___ \   | |  |  __| |  _  / 
# | |  | |/ ____ \ ____) |  | |  | |____| | \ \ 
# |_|  |_/_/    \_\_____/   |_|  |______|_|  \_\
                                                                                           
cmake_minimum_required (VERSION 4.0.2)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE CXX_EXTENSIONS OFF)

project ("DoctrinaSDK")

add_compile_definitions("RELEASE=$<CONFIG:Release>")
add_compile_definitions("DEBUG=$<CONFIG:Debug>")

add_subdirectory ("DoctrinaEngine")
add_subdirectory ("DoctrinaEditor")

include(FetchContent)

FetchContent_Declare(
  sdl2
  GIT_REPOSITORY https://github.com/libsdl-org/SDL
  GIT_TAG release-2.32.2
)

FetchContent_MakeAvailable(sdl2)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()