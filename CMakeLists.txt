cmake_minimum_required(VERSION 3.5.0)
project(SimrailController VERSION 0.1.0 LANGUAGES C)

include(CTest)
enable_testing()

find_package(SDL2 REQUIRED)
find_package(X11 REQUIRED)

find_library(XTEST_LIBRARY NAMES Xtst)

add_executable(SimrailController main.c EU07.c functions.c)

target_include_directories(SimrailController PRIVATE ${SDL2_INCLUDE_DIRS} ${X11_INCLUDE_DIRS})
target_link_libraries(SimrailController PRIVATE ${SDL2_LIBRARIES} ${X11_LIBRARIES} ${XTEST_LIBRARY})


set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
