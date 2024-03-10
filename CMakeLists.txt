cmake_minimum_required(VERSION 3.26)
project(TrainTicketingSystem C)

set(CMAKE_C_STANDARD 23)

if (WIN32 OR MSVC)
    add_compile_definitions("_CRT_SECURE_NO_WARNINGS")
endif ()

add_executable(TrainTicketingSystem
        src/main.c
        src/utils.c)
target_include_directories(TrainTicketingSystem PRIVATE include/)

# OpenSSL
target_include_directories(TrainTicketingSystem PRIVATE include/openssl/include/)
find_package(OpenSSL REQUIRED)
if (OpenSSL_FOUND)
    target_link_libraries(TrainTicketingSystem PRIVATE OpenSSL::SSL)
    message(STATUS "OpenSSL Found:")
    message(STATUS " INTL_INCLUDE_DIRS: ${OpenSSL_INCLUDE_DIRS}")
    message(STATUS " INTL_LIBRARIES: ${OpenSSL_LIBRARIES}")
    message(STATUS " Version: ${OpenSSL_VERSION}")
else ()
    message(STATUS "OpenSSL Not Found!")
endif ()