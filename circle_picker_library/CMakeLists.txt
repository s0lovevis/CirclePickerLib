cmake_minimum_required(VERSION 3.24)
project(CirclePickerLibrary)

set(CMAKE_CXX_STANDARD 17)

add_library(${PROJECT_NAME} STATIC)

set(SOURCE_FILES
		${CMAKE_CURRENT_SOURCE_DIR}/src/circle_picker.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/src/geometry_operations.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/src/info_messages.cpp
)

target_sources(${PROJECT_NAME}
		PUBLIC
		${SOURCE_FILES}
)

target_include_directories(${PROJECT_NAME}
		PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR}/src
		PUBLIC
		${CMAKE_CURRENT_SOURCE_DIR}/include
)


find_package(OpenCV REQUIRED)
target_link_libraries(CirclePickerLibrary ${OpenCV_LIBS})

file(MAKE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib)

install(TARGETS ${PROJECT_NAME}
		ARCHIVE DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/lib)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/
		DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/lib/include)