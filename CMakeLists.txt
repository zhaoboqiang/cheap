cmake_minimum_required(VERSION 3.0.0)
project(cheap VERSION 0.0.0)

add_subdirectory(test)

include(CTest)
enable_testing()

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
