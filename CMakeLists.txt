
#  __  __           _____ _______ ______ _____  
# |  \/  |   /\    / ____|__   __|  ____|  __ \ 
# | \  / |  /  \  | (___    | |  | |__  | |__) |
# | |\/| | / /\ \  \___ \   | |  |  __| |  _  / 
# | |  | |/ ____ \ ____) |  | |  | |____| | \ \ 
# |_|  |_/_/    \_\_____/   |_|  |______|_|  \_\
                                                                                           
cmake_minimum_required (VERSION 3.8)

project ("DoctrinaSDK")

add_subdirectory ("DoctrinaEngine")
add_subdirectory ("Editor")

target_include_directories(Editor PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/")

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()