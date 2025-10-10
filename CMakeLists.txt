cmake_minimum_required(VERSION 3.10)
project(MyProject VERSION 1.0.0 LANGUAGES C)

find_package(OpenSSL REQUIRED)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(GLOB LIB_SOURCES "src/lib/*.c")

include_directories(${Openssl_INCLUDE_DIR})

add_library(mylib STATIC ${LIB_SOURCES})

target_include_directories(mylib PUBLIC
    ${CMAKE_SOURCE_DIR}/include
)

# compiling options are here
target_compile_options(mylib PRIVATE -Wall -Wextra)

file(GLOB APP_SOURCES "src/apps/*.c")

# create an executable file for each .c in apps/
foreach(app_source ${APP_SOURCES})
    get_filename_component(app_name ${app_source} NAME_WE)

    add_executable(${app_name} ${app_source})

    target_link_libraries(${app_name} OpenSSL::Crypto)

    target_link_libraries(${app_name} mylib)

    target_include_directories(${app_name} PRIVATE
        ${CMAKE_SOURCE_DIR}/include
    )

    message(STATUS "added executable: ${app_name}")
endforeach()
