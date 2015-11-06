cmake_minimum_required(VERSION 3.2.1)

project(Pong)

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

set(SOURCES_PONG Pong.cpp Pong.h)
source_group("Pong" FILES ${SOURCES_PONG})

add_executable(Pong WIN32 ${SOURCES_MAIN} ${SOURCES_PONG})
set_property(TARGET Pong APPEND PROPERTY COMPILE_DEFINITIONS "SH_DEV=1")
set_property(TARGET Pong APPEND PROPERTY COMPILE_DEFINITIONS "SH_MASTER=1")
target_link_libraries(Pong ${SH_SDK_LIB})
target_link_libraries(Pong ${DirectX9_LIBRARIES} ${XInput_LIBRARIES} ${XAudio_LIBRARY})

