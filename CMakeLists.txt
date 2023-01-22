cmake_minimum_required(VERSION 3.16.3)
SET(PROJECT_NAME JungleGame)


project(${PROJECT_NAME} C)

set(CMAKE_C_STANDARD 11)

file(GLOB_RECURSE HEADERS_FILES
        ${PROJECT_SOURCE_DIR}/headers/*.h
        )
file(GLOB_RECURSE SOURCES_FILES
        ${PROJECT_SOURCE_DIR}/sources/*.c
        )

SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

add_executable(${CMAKE_PROJECT_NAME} main.c ${SOURCES_FILES} ${HEADERS_FILES})

find_package (SQLite3)
if (SQLITE3_FOUND)
    include_directories(${SQLITE3_INCLUDE_DIRS})
    target_link_libraries (tutorial ${SQLITE3_LIBRARIES})
    MESSAGE("SQLITE3 HAS BEEN FOUND")
else ()
    MESSAGE("SQLITE3 HAS NOT BEEN FOUND")
endif (SQLITE3_FOUND)

target_link_libraries(${CMAKE_PROJECT_NAME} -lws2_32)