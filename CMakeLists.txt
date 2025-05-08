
#  __  __           _____ _______ ______ _____  
# |  \/  |   /\    / ____|__   __|  ____|  __ \ 
# | \  / |  /  \  | (___    | |  | |__  | |__) |
# | |\/| | / /\ \  \___ \   | |  |  __| |  _  / 
# | |  | |/ ____ \ ____) |  | |  | |____| | \ \ 
# |_|  |_/_/    \_\_____/   |_|  |______|_|  \_\
                                                                                           
cmake_minimum_required (VERSION 4.0.2)

project ("DoctrinaSDK")

add_compile_definitions("RELEASE=$<CONFIG:Release>")
add_compile_definitions("DEBUG=$<CONFIG:Debug>")

add_subdirectory ("DoctrinaEngine")
add_subdirectory ("Editor")

include(FetchContent)

FetchContent_Declare(
  sdl2
  GIT_REPOSITORY https://github.com/libsdl-org/SDL
  GIT_TAG release-2.32.2
)

FetchContent_MakeAvailable(sdl2)

#DELETE THE OUT FOLDER AFTER CHANGING THIS VARIABLE
option(PRODUCTION_BUILD "Remove console and debug signatures" off)

if(PRODUCTION_BUILD)
	if(MSVC)

	#Remove console and debug signatures

		target_compile_definitions(DoctrinaEngine PUBLIC _CRT_SECURE_NO_WARNINGS)
		target_compile_definitions(Editor PUBLIC _CRT_SECURE_NO_WARNINGS)


		set_target_properties(DoctrinaEngine PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
		set_target_properties(Editor PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")

	endif()
endif()

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()