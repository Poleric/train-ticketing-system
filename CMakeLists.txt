cmake_minimum_required(VERSION 3.26)
project(TrainTicketingSystem C)

set(CMAKE_C_STANDARD 23)

# remove warning for unsafe functions.
if (WIN32 OR MSVC)
    add_compile_definitions("_CRT_SECURE_NO_WARNINGS")
    add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")  # utf-8 things
    add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
endif ()

set (SOURCES
    ${CMAKE_SOURCE_DIR}/src/main.c
    ${CMAKE_SOURCE_DIR}/src/utils.c
)

add_executable(TrainTicketingSystem ${SOURCES})
target_include_directories(TrainTicketingSystem PRIVATE include/)

# OpenSSL
target_include_directories(TrainTicketingSystem PRIVATE include/openssl/include/)
find_package(OpenSSL REQUIRED)
if (OpenSSL_FOUND)
    target_link_libraries(TrainTicketingSystem PRIVATE OpenSSL::SSL)
    message(STATUS "OpenSSL Found")
else ()
    message(STATUS "OpenSSL Not Found!")
endif ()