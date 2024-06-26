cmake_minimum_required(VERSION 3.5.0)
project(SimrailController VERSION 0.1.0 LANGUAGES C)

include(CTest)
enable_testing()

if (WIN32)
  if(MINGW)
    set(SDL2_INCLUDE_DIRS "${CMAKE_CURRENT_LIST_DIR}/SDL2-2.30.2/x86_64-w64-mingw32/include")
    set(SDL2_LIBRARIES "${CMAKE_CURRENT_LIST_DIR}/SDL2-2.30.2/x86_64-w64-mingw32/lib/libSDL2.a;${CMAKE_CURRENT_LIST_DIR}/SDL2-2.30.2/x86_64-w64-mingw32/lib/libSDL2main.a")
    string(STRIP "${SDL2_LIBRARIES}" SDL2_LIBRARIES)
    set(MANIFEST_FILE ${CMAKE_SOURCE_DIR}/CustomControllerToSimrail.manifest)
  else()
    set(SDL2_INCLUDE_DIRS "${CMAKE_CURRENT_LIST_DIR}/include")
  
    if (${CMAKE_SIZEOF_VOID_P} MATCHES 8)
        set(SDL2_LIBRARIES "${CMAKE_CURRENT_LIST_DIR}/lib/x64/SDL2.lib;${CMAKE_CURRENT_LIST_DIR}/lib/x64/SDL2main.lib")
    else ()
        set(SDL2_LIBRARIES "${CMAKE_CURRENT_LIST_DIR}/lib/x86/SDL2.lib;${CMAKE_CURRENT_LIST_DIR}/lib/x86/SDL2main.lib")
    endif ()
  
    string(STRIP "${SDL2_LIBRARIES}" SDL2_LIBRARIES)
  endif()
else()
    find_package(SDL2 REQUIRED)
    find_package(X11 REQUIRED)
    find_library(XTEST_LIBRARY NAMES Xtst)
endif()

add_executable(SimrailController main.c EU07.c ET22.c functions.c)

target_include_directories(SimrailController PRIVATE ${SDL2_INCLUDE_DIRS})
target_link_libraries(SimrailController PRIVATE ${SDL2_LIBRARIES})

if (WIN32)
  if (MINGW)
    target_link_libraries(SimrailController PRIVATE m dinput8 m dinput8 dxguid dxerr8 user32 gdi32 winmm imm32 ole32 oleaut32 shell32 setupapi version uuid mingw32)
  endif()
  target_sources(SimrailController PRIVATE ${MANIFEST_FILE})
else()
  target_include_directories(SimrailController PRIVATE ${X11_INCLUDE_DIRS})
  target_link_libraries(SimrailController PRIVATE ${X11_LIBRARIES} ${XTEST_LIBRARY})
endif()

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)