cmake_minimum_required(VERSION 3.7)

add_definitions(-std=c++17)

set(CXX_FLAGS "-Wall")
set(CMAKE_CXX_FLAGS, "${CXX_FLAGS}")

project(Snaker)

file(GLOB_RECURSE Snaker_SOURCES "src/*.cpp")
file(GLOB_RECURSE Snaker_HEADERS "src/*.h")

set (Snaker_INCLUDE_DIRS "")
foreach (_headerFile ${Snaker_HEADERS})
    get_filename_component(_dir ${_headerFile} PATH)
    list (APPEND Snaker_INCLUDE_DIRS ${_dir})
endforeach()
list(REMOVE_DUPLICATES Snaker_INCLUDE_DIRS)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)


include_directories(${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS} ${SDL2_TTF_LIBRARIES} ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(Snaker ${Snaker_SOURCES})
string(STRIP "${SDL2_LIBRARIES}" SDL2_LIBRARIES)
target_link_libraries(Snaker ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES} ${SDL2_TTF_LIBRARIES})
