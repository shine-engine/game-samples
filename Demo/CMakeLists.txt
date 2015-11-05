cmake_minimum_required(VERSION 3.2.1)

project(Demo)

set(SH_ROOT_DIR "../Shine" CACHE PATH "Shine root directory")

include_directories("${SH_ROOT_DIR}/include")

find_library(SH_SDK_LIB "ShSDK" PATHS "${SH_ROOT_DIR}" PATH_SUFFIXES Lib lib NO_DEFAULT_PATH)

if (WIN32)
	find_package(DirectX9 REQUIRED)
	include_directories("${DirectX9_INCLUDE_DIR}")

	find_package(XAudio REQUIRED)
	find_package(XInput REQUIRED)

	add_definitions(-D_ITERATOR_DEBUG_LEVEL=0)
endif (WIN32)

set(SOURCES_MAIN main.cpp)
source_group("" FILES ${SOURCES_MAIN})

set(SOURCES_PONG Demo.cpp Demo.h)
source_group("Demo" FILES ${SOURCES_PONG})

add_executable(Demo WIN32 ${SOURCES_MAIN} ${SOURCES_PONG})
set_property(TARGET Demo APPEND PROPERTY COMPILE_DEFINITIONS "SH_DEV=1")
set_property(TARGET Demo APPEND PROPERTY COMPILE_DEFINITIONS "SH_MASTER=1")
target_link_libraries(Demo ${SH_SDK_LIB})
target_link_libraries(Demo ${DirectX9_LIBRARIES} ${XInput_LIBRARIES} ${XAudio_LIBRARY})

