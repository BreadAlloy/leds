cmake_minimum_required(VERSION 3.0.0)

# read and parse version file

project(rpi_ws281x VERSION "1.1.0")

option(BUILD_SHARED "Build as shared library" OFF)
option(BUILD_TEST "Build test application" ON)

SET(CMAKE_C_FLAGS  "${CMAKE_C_FLAGS} -g -O3 -Wall")

set(CMAKE_C_STANDARD 11)

set(LIB_TARGET ws2811)
set(TEST_TARGET test)
set(TESTCPP_TARGET testcpp)

set(LIB_PUBLIC_HEADERS
    ws2811.h
    rpihw.h
    pwm.h
    clk.h
    dma.h
    gpio.h
    mailbox.h
    pcm.h
    simpleleds.h
)

set(LIB_SOURCES
    mailbox.c
    ws2811.c
    pwm.c
    pcm.c
    dma.c
    rpihw.c
    simpleleds.c
)

set(TEST_SOURCES
    main.c
)

set(TESTCPP_SOURCES
    testcpp.cpp
)

include(GNUInstallDirs)

configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/pkg-config.pc.in"
    "${CMAKE_CURRENT_BINARY_DIR}/libws2811.pc"
    @ONLY
)

set(DEST_HEADERS_DIR include/ws2811)
set(DEST_LIB_DIR lib)

if(BUILD_SHARED)
    add_library(${LIB_TARGET} SHARED ${LIB_SOURCES})
else()
    add_library(${LIB_TARGET} ${LIB_SOURCES})
endif()

target_link_libraries(${LIB_TARGET} m pthread)
set_target_properties(${LIB_TARGET} PROPERTIES PUBLIC_HEADER "${LIB_PUBLIC_HEADERS}")

install(TARGETS ${LIB_TARGET}
    ARCHIVE DESTINATION ${DEST_LIB_DIR}
    PUBLIC_HEADER DESTINATION ${DEST_HEADERS_DIR}
)

INSTALL(FILES "${CMAKE_CURRENT_BINARY_DIR}/libws2811.pc"
        DESTINATION lib/pkgconfig)

install(CODE "execute_process(COMMAND /sbin/ldconfig
                              RESULT_VARIABLE EXIT_STATUS
                              ERROR_QUIET)
             if (NOT EXIT_STATUS EQUAL 0)
                 message(\"Warning: Could not run ldconfig. You may need to manually run ldconfig as root to cache the newly installed libraries.\")
             endif()")

if(BUILD_TEST)
    include_directories(${CMAKE_CURRENT_BINARY_DIR})

    add_executable(${TEST_TARGET} ${TEST_SOURCES})
    target_link_libraries(${TEST_TARGET} ${LIB_TARGET})
    add_executable(${TESTCPP_TARGET} ${TESTCPP_SOURCES})
    target_link_libraries(${TESTCPP_TARGET} ${LIB_TARGET})
endif()
