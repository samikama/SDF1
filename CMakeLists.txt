cmake_minimum_required(3.17)
procject(SDF1 CXX)
option(ENABLE_DVLOG "Enable debug verbose logging")

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

if(NOT DEFINED CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE RelWithDebInfo)
endif()

if(ENABLE_DVLOG)
  add_compile_definitions("ENABLE_DVLOG=1")
endif()
add_subdirectory(pybind11)
find_package(ZLIB REQUIRED)
find_package(Threads REQUIRED)
include_directories(BEFORE ${PROJECT_SOURCE_DIR})
include(CMessage)
# define basedir length to remove from file paths for logs
string(LENGTH "${PROJECT_SOURCE_DIR}/" SOURCE_PREFIX_LENGTH )
add_definitions("-DSOURCE_PREFIX_LENGTH=${SOURCE_PREFIX_LENGTH}")
