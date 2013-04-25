cmake_minimum_required(VERSION 2.8)
if(COMMAND CMAKE_POLICY)
  cmake_policy(SET CMP0003 NEW)
endif()


project(itkMultiLevelSetEvolution)

find_package(ITK REQUIRED)
include(${ITK_USE_FILE})

add_executable(itkMultiLevelSetEvolution itkMultiLevelSetEvolution.cxx )
target_link_libraries(itkMultiLevelSetEvolution ${ITK_LIBRARIES})
