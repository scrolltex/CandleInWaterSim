cmake_minimum_required(VERSION 3.1)

set(CMAKE_CXX_STANDARD 11)

project(CandleInWaterSim)

# Define sources and executable
set(EXECUTABLE_NAME "CandleInWaterSim")
set(SRC src/main.cpp 
        src/materials.cpp 
        src/point.cpp 
        src/candle.cpp
		src/gui.cpp
)

add_executable(${EXECUTABLE_NAME} ${SRC})

#set(SFML_STATIC_LIBRARIES (SFML_OS_WINDOWS AND SFML_COMPILER_MSVC))

# Detect and add SFML
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})

# Find sfml (also look for the main component when using Visual Studio)
if(MSVC)
    set(SFML_STATIC_LIBRARIES TRUE)
    find_package(SFML 2 COMPONENTS main graphics window system REQUIRED)
else()
    set(SFML_STATIC_LIBRARIES FALSE)
    find_package(SFML 2 COMPONENTS graphics window system REQUIRED)
endif()

find_package(TGUI 0.7 REQUIRED)

# FindSFML couldn't find SFML.
if(NOT SFML_FOUND)
    set(SFML_ROOT "" CACHE PATH "SFML root directory")
    message(FATAL_ERROR "CMake couldn't find SFML. Set the SFML_ROOT entry to SFML's root directory (containing \"include\" and \"lib\" directories).")
endif()

# FindTGUI couldn't find TGUI.
if(NOT TGUI_FOUND)
    set(TGUI_ROOT "" CACHE PATH "TGUI root directory")
    message(FATAL_ERROR "CMake couldn't find TGUI. Set the TGUI_ROOT entry to TGUI's root directory (containing \"include\" and \"lib\" directories).")
endif()

# OpenMP
find_package(OpenMP)
if(OPENMP_FOUND)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()

include_directories(${SFML_INCLUDE_DIR} ${TGUI_INCLUDE_DIR})
target_link_libraries(${EXECUTABLE_NAME} ${SFML_LIBRARIES} ${SFML_DEPENDENCIES} ${TGUI_LIBRARY})
