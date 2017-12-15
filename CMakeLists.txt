cmake_minimum_required(VERSION 2.6)

# Define sources and executable
set(EXECUTABLE_NAME "CandleInWaterSim")
add_executable(${EXECUTABLE_NAME} src/main.cpp 
                                  src/materials.cpp 
                                  src/point.cpp 
                                  src/matrix.cpp)

# Detect and add SFML
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})
#Find any version 2.X of SFML
#See the FindSFML.cmake file for additional details and instructions
find_package(SFML 2 REQUIRED graphics window system)
if(SFML_FOUND)
  include_directories(${SFML_INCLUDE_DIR})
  target_link_libraries(${EXECUTABLE_NAME} ${SFML_LIBRARIES} ${SFML_DEPENDENCIES})
endif()