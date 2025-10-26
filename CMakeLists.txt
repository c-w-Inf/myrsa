cmake_minimum_required(VERSION 3.10)
project(myrsa)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -Wall")
endif()

file(GLOB_RECURSE BOOST_HEADERS "ext_inc/*.hpp")
add_library(boost INTERFACE)
target_include_directories(boost INTERFACE
    ${CMAKE_SOURCE_DIR}/ext_inc
)
target_sources(boost INTERFACE ${BOOST_HEADERS})

file(GLOB_RECURSE LIB_SOURCES "src/lib/*.cpp")
add_library(myrsa_lib STATIC ${LIB_SOURCES})
target_include_directories(myrsa_lib PUBLIC
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/ext_inc
)

# compiling options are here
target_compile_options(myrsa_lib PRIVATE -Wall -Wextra)

file(GLOB APP_SOURCES "src/apps/*.cpp")

# create an executable file for each .cpp in apps/
foreach(app_source ${APP_SOURCES})
    get_filename_component(app_name ${app_source} NAME_WE)

    add_executable(${app_name} ${app_source})

    target_link_libraries(${app_name} myrsa_lib boost)

    message(STATUS "added executable: ${app_name}")
endforeach()
