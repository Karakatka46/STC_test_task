cmake_minimum_required(VERSION 3.16)
project(algoritm_Karplus)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(UNIX AND NOT APPLE)
    find_program(PLAY_EXECUTABLE play)
    if(NOT PLAY_EXECUTABLE)
        message(WARNING "SoX (play command) not found. Audio playback will not be available.")
        message(STATUS "Install with: sudo apt-get install sox")
    else()
        message(STATUS "Found SoX audio tools: ${PLAY_EXECUTABLE}")
    endif()
endif()

set(SOURCES
    main.cpp
    algKarplus.cpp
    )

include_directories(include)
add_executable(Karplus_app ${SOURCES})

if(UNIX AND NOT APPLE)
    install(TARGETS Karplus_app DESTINATION bin)
    install(FILES ${CMAKE_SOURCE_DIR}/autorun.sh DESTINATION bin PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ)
endif()