cmake_minimum_required(VERSION 3.3)
project(CG_RayTracing)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wno-deprecated")

set(SOURCE_FILES bimage.cpp ex5.cpp scene.cpp MyMeshObject.cpp polygon.cpp sphere.cpp triangle.cpp camera.cpp object.cpp)

find_package(GLEW)
find_package(GLUT)
#find_path(OpenMeshCore /cs/course/2013/cg/)
#find_path(OPENGL /cs/course/current/cg/)
#find_path(glm /cs/course/current/cg/)
#
#include_directories(${OPENGL_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIRS} ${GLEW_INCLUDE_DIRS}
#        ${GLEW_INCLUDE_DIRS} ${OpenMeshCore_INCLUDE_DIRS})
#link_libraries(${OPENGL_LIBRARIES} ${GLUT_LIBRARIES} ${GLEW_LIBRARIES} ${glm_LIBRARIES} ${OpenMeshCore_LIBRARIES})
link_directories(/usr/local/Cellar/open-mesh/4.1/lib)
include_directories(/usr/local/Cellar/open-mesh/4.1/include)
add_executable(CG_RayTracing ${SOURCE_FILES})
target_link_libraries(CG_RayTracing dl OpenMeshCore)
#link_directories(/cs/course/2013/cg/OpenMesh/lib)
#add_executable(CG_RayTracing ${SOURCE_FILES})
#include_directories(/cs/course/2013/cg/OpenMesh/include)
#target_link_libraries(CG_RayTracing GLEW glut GLU GL dl OpenMeshCore)
#=======
#
#project(ex3)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wno-deprecated")
#
#set(SOURCE_FILES ex5.cpp bimage.cpp)
#
#find_package(GLEW REQUIRED)
#find_package(GLUT REQUIRED)
#find_package(OPENGL REQUIRED)
#find_package(glm REQUIRED)
#
#find_path(OPEN_MESH_INCLUDES)
#
#include_directories(${OPENGL_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIRS} ${GLEW_INCLUDE_DIRS}
#        ${glm_INCLUDE_DIRS})
#link_libraries(${OPENGL_LIBRARIES} ${GLUT_LIBRARIES} ${GLEW_LIBRARIES} ${glm_LIBRARIES})
#add_executable(ex3 ${SOURCE_FILES})
#>>>>>>> Stashed changes
