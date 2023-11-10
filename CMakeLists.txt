cmake_minimum_required(VERSION 3.24)
project(circle_picker)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

link_directories(".\\circle_picker_library\\lib")
include_directories(".\\circle_picker_library\\include")

add_executable(circle_picker test.cpp)

find_package(OpenCV REQUIRED)
target_link_libraries(circle_picker CirclePickerLibrary ${OpenCV_LIBS})