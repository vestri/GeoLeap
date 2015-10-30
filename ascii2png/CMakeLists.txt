cmake_minimum_required (VERSION 2.6)
project(ASCII2PNG)

#Inclusion de Boost ad OpenCV
include(FindBoost)
find_package( Boost REQUIRED)
find_package( OpenCV REQUIRED)

link_directories(${Boost_LIBRARY_DIRS})
include_directories(${Boost_INCLUDE_DIRS})
link_directories(${OpenCV_LIBRARY_DIRS})
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(Ascii2png ascii2png.cc)
target_link_libraries( Ascii2png ${OpenCV_LIBS} )