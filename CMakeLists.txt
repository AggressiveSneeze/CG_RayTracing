cmake_minimum_required(VERSION 3.3)
project(CG_RayTracing)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wno-deprecated")

set(SOURCE_FILES ex5.cpp bimage.cpp sphere.cpp MyMeshObject.cpp camera.cpp)

#find_package(GLEW)
#find_package(GLUT)
#find_path(OpenMeshCore /cs/course/2013/cg/)
#find_path(OPENGL /cs/course/current/cg/)
#find_path(glm /cs/course/current/cg/)
#
#include_directories(${OPENGL_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIRS} ${GLEW_INCLUDE_DIRS}
#        ${GLEW_INCLUDE_DIRS} ${OpenMeshCore_INCLUDE_DIRS})
#link_libraries(${OPENGL_LIBRARIES} ${GLUT_LIBRARIES} ${GLEW_LIBRARIES} ${glm_LIBRARIES} ${OpenMeshCore_LIBRARIES})

link_directories(/cs/course/2013/cg/OpenMesh/lib)
add_executable(CG_RayTracing ${SOURCE_FILES})
include_directories(/cs/course/2013/cg/OpenMesh/include)
target_link_libraries(CG_RayTracing GLEW glut GLU GL dl OpenMeshCore)