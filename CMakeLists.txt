cmake_minimum_required (VERSION 3.8)

project ("diplom_project")

set(MY_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/inc)
set(MY_SRC_DIR ${CMAKE_SOURCE_DIR}/src)

include(FetchContent)
set(JSON_BuildTests OFF CACHE INTERNAL "")
FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz)
FetchContent_MakeAvailable(json)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG release-1.12.1
)

FetchContent_MakeAvailable(googletest)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
enable_testing()


# TODO: Добавьте тесты и целевые объекты, если это необходимо.
add_subdirectory(src)
add_subdirectory(tests)