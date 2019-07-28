cmake_minimum_required(VERSION 3.5)
project(wadtool)

set(TARGET wadtool)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)
set(LIB_WAD_DIR ${CMAKE_SOURCE_DIR}/libwad)
set(TARGET_INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/headers ${LIB_WAD_DIR})

find_package(GTK2 REQUIRED)
if(NOT GTK2_FOUND)
	message(SEND_ERROR, "Failed to find gtk2")
	return()
endif()

file(GLOB SOURCE ${CMAKE_SOURCE_DIR}/src/*.c)
add_executable(${TARGET} ${SOURCE})

target_include_directories(${TARGET} PUBLIC ${GTK2_INCLUDE_DIRS} ${TARGET_INCLUDE_DIRS})

add_subdirectory(${LIB_WAD_DIR})
target_link_libraries(${TARGET} ${GTK2_LIBRARIES} libwad)
