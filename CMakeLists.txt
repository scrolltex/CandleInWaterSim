cmake_minimum_required(VERSION 3.0)

project(CandleInWaterSim)

# Define sources and executable
set(EXECUTABLE_NAME "CandleInWaterSim")
set(SRC src/main.cpp 
        src/materials.cpp 
        src/point.cpp 
        src/matrix.cpp
)

add_executable(${EXECUTABLE_NAME} ${SRC})

set(SFML_STATIC_LIBRARIES TRUE)

# Detect and add SFML
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})

# Find sfml (also look for the main component when using Visual Studio)
if(SFML_OS_WINDOWS AND SFML_COMPILER_MSVC)
    find_package(SFML 2 COMPONENTS main graphics window system REQUIRED)
else()
    find_package(SFML 2 COMPONENTS graphics window system REQUIRED)
endif()

# FindSFML couldn't find SFML.
if(NOT SFML_FOUND)
    set(SFML_ROOT "" CACHE PATH "SFML root directory")
    message(FATAL_ERROR "CMake couldn't find SFML. Set the SFML_ROOT entry to SFML's root directory (containing \"include\" and \"lib\" directories).")
endif()

include_directories(${SFML_INCLUDE_DIR})
target_link_libraries(${EXECUTABLE_NAME} ${SFML_LIBRARIES} ${SFML_DEPENDENCIES})
