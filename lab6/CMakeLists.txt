cmake_minimum_required(VERSION 3.5.0)
project(lab6)

include_directories( include ../ )

find_package( OpenCV REQUIRED )
include_directories(${OpenCV_INCLUDE_DIRS})

set(sources "")
file(
        GLOB_RECURSE

        sources

        "${CMAKE_CURRENT_LIST_DIR}/*.h"
        "${CMAKE_CURRENT_LIST_DIR}/*.hpp"
        "${CMAKE_CURRENT_LIST_DIR}/*.hpp.inl"
        "${CMAKE_CURRENT_LIST_DIR}/*.cpp"
)


add_executable(${PROJECT_NAME} ${sources})
add_custom_command(TARGET lab6 
                   POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:lab6> ${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(lab6 ${OpenCV_LIBS})

set(CMAKE_CXX_STANDARD 20)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
if (MSVC)
   set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /STACK:10000000")
else ()
   set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--stack,10000000")
endif()

include(CPack)