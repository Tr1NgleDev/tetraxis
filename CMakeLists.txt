cmake_minimum_required(VERSION 3.21)
project(Tetraxis)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE SRC_FILES "src/*.cpp")
file(GLOB_RECURSE HEADER_FILES "include/*.h")

add_executable(tetraxis ${SRC_FILES} ${HEADER_FILES} icons/icon.rc)

target_include_directories(tetraxis
	PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_include_directories(tetraxis
	PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR}/libs
)

find_package(OpenGL REQUIRED)
target_link_libraries(tetraxis PRIVATE ${OPENGL_LIBRARIES})

find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(tetraxis PRIVATE glfw)

find_package(GLEW REQUIRED)
target_link_libraries(tetraxis PRIVATE GLEW::GLEW)

find_package(glm CONFIG REQUIRED)
target_link_libraries(tetraxis PRIVATE glm::glm-header-only)
target_compile_definitions(tetraxis PRIVATE GLM_EXPERIMENTAL)

add_subdirectory("libs/ndtf")
target_link_libraries(tetraxis PRIVATE ndtf)

add_subdirectory("libs/libresvg")
target_link_libraries(tetraxis PRIVATE resvg)

if(MSVC)
	add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

if(WIN32)
	install(IMPORTED_RUNTIME_ARTIFACTS glfw DESTINATION .)
	install(IMPORTED_RUNTIME_ARTIFACTS GLEW::GLEW DESTINATION .)
	install(IMPORTED_RUNTIME_ARTIFACTS resvg DESTINATION .)

	install(TARGETS tetraxis RUNTIME DESTINATION .)

	install(DIRECTORY ${CMAKE_SOURCE_DIR}/assets DESTINATION .)

elseif(LINUX)
	install(IMPORTED_RUNTIME_ARTIFACTS resvg DESTINATION .)

	install(TARGETS tetraxis RUNTIME DESTINATION lib/tetraxis RENAME tetraxis)
	install(DIRECTORY ${CMAKE_SOURCE_DIR}/assets DESTINATION lib/tetraxis)
	
	install(SCRIPT ${CMAKE_SOURCE_DIR}/cmake/SymLinkBin.cmake)
	
	install(FILES ${CMAKE_SOURCE_DIR}/cmake/dev.tr1ngle.tetraxis.desktop
		DESTINATION share/applications
	)
	
	install(FILES ${CMAKE_SOURCE_DIR}/LICENSE DESTINATION share/licenses/tetraxis)
	install(FILES ${CMAKE_SOURCE_DIR}/icons/sizes/icon_16.png DESTINATION share/icons/hicolor/16x16/apps RENAME dev.tr1ngle.tetraxis.png)
	install(FILES ${CMAKE_SOURCE_DIR}/icons/sizes/icon_22.png DESTINATION share/icons/hicolor/22x22/apps RENAME dev.tr1ngle.tetraxis.png)
	install(FILES ${CMAKE_SOURCE_DIR}/icons/sizes/icon_24.png DESTINATION share/icons/hicolor/24x24/apps RENAME dev.tr1ngle.tetraxis.png)
	install(FILES ${CMAKE_SOURCE_DIR}/icons/sizes/icon_32.png DESTINATION share/icons/hicolor/32x32/apps RENAME dev.tr1ngle.tetraxis.png)
	install(FILES ${CMAKE_SOURCE_DIR}/icons/sizes/icon_36.png DESTINATION share/icons/hicolor/36x36/apps RENAME dev.tr1ngle.tetraxis.png)
	install(FILES ${CMAKE_SOURCE_DIR}/icons/sizes/icon_48.png DESTINATION share/icons/hicolor/48x48/apps RENAME dev.tr1ngle.tetraxis.png)
	install(FILES ${CMAKE_SOURCE_DIR}/icons/sizes/icon_64.png DESTINATION share/icons/hicolor/64x64/apps RENAME dev.tr1ngle.tetraxis.png)
	install(FILES ${CMAKE_SOURCE_DIR}/icons/sizes/icon_72.png DESTINATION share/icons/hicolor/72x72/apps RENAME dev.tr1ngle.tetraxis.png)
	install(FILES ${CMAKE_SOURCE_DIR}/icons/sizes/icon_96.png DESTINATION share/icons/hicolor/96x96/apps RENAME dev.tr1ngle.tetraxis.png)
	install(FILES ${CMAKE_SOURCE_DIR}/icons/sizes/icon_128.png DESTINATION share/icons/hicolor/128x128/apps RENAME dev.tr1ngle.tetraxis.png)
	install(FILES ${CMAKE_SOURCE_DIR}/icons/sizes/icon_192.png DESTINATION share/icons/hicolor/192x192/apps RENAME dev.tr1ngle.tetraxis.png)
	install(FILES ${CMAKE_SOURCE_DIR}/icons/sizes/icon_256.png DESTINATION share/icons/hicolor/256x256/apps RENAME dev.tr1ngle.tetraxis.png)
	install(FILES ${CMAKE_SOURCE_DIR}/icons/sizes/icon_384.png DESTINATION share/icons/hicolor/384x384/apps RENAME dev.tr1ngle.tetraxis.png)
	install(FILES ${CMAKE_SOURCE_DIR}/icons/sizes/icon_512.png DESTINATION share/icons/hicolor/512x512/apps RENAME dev.tr1ngle.tetraxis.png)
	install(FILES ${CMAKE_SOURCE_DIR}/icons/icon.svg DESTINATION share/icons/hicolor/scalable/apps RENAME dev.tr1ngle.tetraxis.svg)
endif()
