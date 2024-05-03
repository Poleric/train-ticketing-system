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

add_compile_definitions(
        MEMBERS_FILEPATH="members.txt"
        STAFFS_FILEPATH="staffs.txt"
        STATIONS_FILEPATH="stations.bin"
        SCHEDULES_FILEPATH="schedules.txt"
        TICKETS_FILEPATH="tickets.bin"
)

if (DEFINED ENV{RECOVERY_EMAIL_FROM} AND DEFINED ENV{RECOVERY_APP_PASSWORD})
    message(STATUS "Optional feature: \"Email recovery\" is enabled.")
    set(EMAIL_RECOVERY_FEATURE ON)
    add_compile_definitions(
        "EMAIL_RECOVERY_FEATURE"
        FROM_MAIL="$ENV{RECOVERY_EMAIL_FROM}"
        APP_PASSWORD="${RECOVERY_APP_PASSWORD}"
    )
else ()
    message(STATUS "Optional feature: \"Email recovery\" is disabled.")
    set(EMAIL_RECOVERY_FEATURE OFF)
endif ()

set (SOURCES
    ${CMAKE_SOURCE_DIR}/src/utils.c

    ${CMAKE_SOURCE_DIR}/src/staff.c
    ${CMAKE_SOURCE_DIR}/src/member.c
    ${CMAKE_SOURCE_DIR}/src/schedule.c
    ${CMAKE_SOURCE_DIR}/src/ticket.c
    ${CMAKE_SOURCE_DIR}/src/stations.c
    ${CMAKE_SOURCE_DIR}/src/recovery_mail.c

    ${CMAKE_SOURCE_DIR}/src/tui/main_menu.c
    ${CMAKE_SOURCE_DIR}/src/tui/member_menu.c
    ${CMAKE_SOURCE_DIR}/src/tui/staff_menu.c
    ${CMAKE_SOURCE_DIR}/src/tui/scheduling_menu.c
    ${CMAKE_SOURCE_DIR}/src/tui/utils/tui_utils.c
    ${CMAKE_SOURCE_DIR}/src/tui/utils/tui_last_pos.c
    ${CMAKE_SOURCE_DIR}/src/tui/utils/menu_utils.c
    ${CMAKE_SOURCE_DIR}/src/tui/form/form.c
    ${CMAKE_SOURCE_DIR}/src/tui/form/login_form.c
    ${CMAKE_SOURCE_DIR}/src/tui/form/register_form.c
    ${CMAKE_SOURCE_DIR}/src/tui/table/table.c

    ${CMAKE_SOURCE_DIR}/src/main.c
)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build)

add_executable(TrainTicketingSystem ${SOURCES})
target_include_directories(TrainTicketingSystem PRIVATE include/)
# math.h
target_link_libraries(TrainTicketingSystem PRIVATE m)

# OpenSSL
find_package(OpenSSL REQUIRED)
if (OpenSSL_FOUND)
    target_link_libraries(TrainTicketingSystem PRIVATE OpenSSL::SSL)
    message(STATUS "OpenSSL found")
else ()
    message(STATUS "OpenSSL not found!")
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
        message(STATUS "Curses not found!")
    endif()
endif ()

# libcurl
if (EMAIL_RECOVERY_FEATURE)
    find_package(CURL REQUIRED)
    if (CURL_FOUND)
        target_link_libraries(TrainTicketingSystem PRIVATE CURL::libcurl)
        message(STATUS "libcurl found")
    else ()
        message(STATUS "libcurl not found!")
    endif ()
endif ()