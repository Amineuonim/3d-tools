cmake_minimum_required(VERSION 3.25)

project(artemis)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Add executable
add_executable(${PROJECT_NAME} main.cpp)

# Find OpenGL, GLUT (FreeGLUT), and GLU
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

# Link OpenGL and GLUT
if(OpenGL_FOUND AND GLUT_FOUND)
    target_link_libraries(${PROJECT_NAME} PUBLIC OpenGL::GL GLUT::GLUT)
else()
    message(FATAL_ERROR "Required libraries (OpenGL or GLUT) not found.")
endif()

# Link GLU explicitly
find_library(GLU_LIBRARY GLU)
if(GLU_LIBRARY)
    target_link_libraries(${PROJECT_NAME} PUBLIC ${GLU_LIBRARY})
else()
    message(FATAL_ERROR "GLU library not found.")
endif()

# Include directories (optional)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/src/includes)

# Set runtime output directory (optional)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
