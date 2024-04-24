cmake_minimum_required(VERSION 3.26)
project(TrainTicketingSystem C)

set(CMAKE_C_STANDARD 23)

# remove warning for unsafe functions.
if (WIN32 OR MSVC)
    add_compile_definitions(
        "_CRT_SECURE_NO_WARNINGS"
        "_CRT_NONSTDC_NO_DEPRECATE"
    )
    add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")  # utf-8 things
    add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
endif ()

set (SOURCES
    ${CMAKE_SOURCE_DIR}/src/utils.c
    ${CMAKE_SOURCE_DIR}/src/staff.c
    ${CMAKE_SOURCE_DIR}/src/member.c

    ${CMAKE_SOURCE_DIR}/src/schedule/stations.c
    ${CMAKE_SOURCE_DIR}/src/schedule/schedule.c

    ${CMAKE_SOURCE_DIR}/src/tui/main_menu.c
    ${CMAKE_SOURCE_DIR}/src/tui/member_menu.c
    ${CMAKE_SOURCE_DIR}/src/tui/staff_menu.c
    ${CMAKE_SOURCE_DIR}/src/tui/scheduling_menu.c
    ${CMAKE_SOURCE_DIR}/src/tui/tui_utils.c
    ${CMAKE_SOURCE_DIR}/src/tui/tui_last_pos.c
    ${CMAKE_SOURCE_DIR}/src/tui/menu_utils.c
    ${CMAKE_SOURCE_DIR}/src/tui/template/form.c
    ${CMAKE_SOURCE_DIR}/src/tui/template/login_form.c

    ${CMAKE_SOURCE_DIR}/src/main.c
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

# ncurses
if (WIN32 OR MSVC)
    message(STATUS "Windows detected. Loading pdcurses instead.")
    find_package(unofficial-pdcurses CONFIG REQUIRED)
    target_link_libraries(TrainTicketingSystem PRIVATE unofficial::pdcurses::pdcurses)
else ()
    set(CURSES_NEED_NCURSES "TRUE")
    set(CURSES_NEED_WIDE "TRUE")
    find_package(Curses REQUIRED)
    if (Curses_FOUND)
        message(STATUS "ncurses found")
        target_link_libraries(TrainTicketingSystem PRIVATE ${CURSES_LIBRARIES})
    else ()
        message(STATUS "Curses Not Found!")
    endif()
endif ()
