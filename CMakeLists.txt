cmake_minimum_required(VERSION 3.27)
project(TrainTicketingSystem C)

set(CMAKE_C_STANDARD 23)

add_executable(TrainTicketingSystem
        src/cli.c)
target_include_directories(TrainTicketingSystem PRIVATE include/)
