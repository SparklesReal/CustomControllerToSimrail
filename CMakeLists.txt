cmake_minimum_required(VERSION 3.5.0)
project(SimrailController VERSION 0.1.0 LANGUAGES C)

include(CTest)
enable_testing()

set(CMAKE_C_COMPILER   x86_64-w64-mingw32)
set(CMAKE_CXX_COMPILER x86_64-w64-mingw32)

if (WIN32)
    set(SDL2_INCLUDE_DIRS "${CMAKE_CURRENT_LIST_DIR}/include")
  
    if (${CMAKE_SIZEOF_VOID_P} MATCHES 8)
        set(SDL2_LIBRARIES "${CMAKE_CURRENT_LIST_DIR}/lib/x64/SDL2.lib;${CMAKE_CURRENT_LIST_DIR}/lib/x64/SDL2main.lib")
    else ()
        set(SDL2_LIBRARIES "${CMAKE_CURRENT_LIST_DIR}/lib/x86/SDL2.lib;${CMAKE_CURRENT_LIST_DIR}/lib/x86/SDL2main.lib")
    endif ()
  
  string(STRIP "${SDL2_LIBRARIES}" SDL2_LIBRARIES)
elseif(MINGW)
  set(SDL2_INCLUDE_DIRS "${CMAKE_CURRENT_LIST_DIR}/lib/x86_64-w64-mingw32/include")
  set(SDL2_LIBRARIES "${CMAKE_CURRENT_LIST_DIR}/lib/x86_64-w64-mingw32/libSDL2.a;${CMAKE_CURRENT_LIST_DIR}/lib/x86_64-w64-mingw32/libSDL2main.a")
  string(STRIP "${SDL2_LIBRARIES}" SDL2_LIBRARIES)
else()
    find_package(X11 REQUIRED)
    find_library(XTEST_LIBRARY NAMES Xtst)
endif()

add_executable(SimrailController main.c EU07.c functions.c)

target_include_directories(SimrailController PRIVATE ${SDL2_INCLUDE_DIRS})
target_link_libraries(SimrailController PRIVATE ${SDL2_LIBRARIES})

if (WIN32 OR MINGW)
else()
  target_include_directories(SimrailController PRIVATE ${X11_INCLUDE_DIRS})
  target_link_libraries(SimrailController PRIVATE ${X11_LIBRARIES} ${XTEST_LIBRARY})
endif()

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
