cmake_minimum_required(VERSION 3.21)

string(TOLOWER "${CMAKE_SYSTEM_PROCESSOR}" ARCH)
if(ARCH STREQUAL "amd64")
	set(ARCH "x86_64")
endif()

if(WIN32)
	set(PLATFORM "windows")
	set(LIB_PREFIX "")
	set(SHARED_LIB_EXT ".dll")
	set(IMPLIB_EXT ".dll.lib")
elseif(LINUX)
	set(PLATFORM "linux")
	set(LIB_PREFIX "lib")
	set(SHARED_LIB_EXT ".so")
	set(IMPLIB_EXT ".so")
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug" OR CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
	set(CONFIG_DIR "Debug")
else()
	set(CONFIG_DIR "Release")
endif()

set(LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/${PLATFORM}/${ARCH}/${CONFIG_DIR}")

add_library(resvg SHARED IMPORTED GLOBAL)

set_target_properties(resvg PROPERTIES
	IMPORTED_LOCATION "${LIB_DIR}/${LIB_PREFIX}resvg${SHARED_LIB_EXT}"
	IMPORTED_IMPLIB "${LIB_DIR}/${LIB_PREFIX}resvg${IMPLIB_EXT}"
	INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/include"
)
