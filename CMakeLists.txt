cmake_minimum_required(VERSION 3.17)
project(SDF1 CXX)
option(ENABLE_DVLOG "Enable debug verbose logging")

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
# include( FetchContent )
# FetchContent_Declare( date_src
#   GIT_REPOSITORY https://github.com/HowardHinnant/date.git
#   GIT_TAG        v3.0.1  # adjust tag/branch/commit as needed
# )
# FetchContent_MakeAvailable(date_src)

# #target_link_libraries (my_target PRIVATE date::date)
include( FetchContent )
FetchContent_Declare( abseil-cpp
  GIT_REPOSITORY https://github.com/abseil/abseil-cpp.git
  GIT_TAG        20230802.rc1  # adjust tag/branch/commit as needed
)
FetchContent_MakeAvailable(abseil-cpp)

#target_link_libraries (my_target PRIVATE date::date)

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
add_library(sdf1_utils SHARED "src/utils/logging.cxx")
add_library(sdf1_core SHARED "src/core/metadata_file.cxx" "src/core/bucket.cxx")
target_link_libraries(sdf1_core PUBLIC sdf1_utils)
target_link_libraries(sdf1_core PRIVATE absl::status)
