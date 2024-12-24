cmake_minimum_required(VERSION 3.10)
project(gtk-glade-test)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Os")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s -Wl,--gc-sections")

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK REQUIRED gtk+-3.0)

find_program(GLIB_COMPILE_RESOURCES NAMES glib-compile-resources REQUIRED)

include_directories(${GTK_INCLUDE_DIRS})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

set(GRESOURCE_DIR_NAME "gres")
set(GRESOURCE_XML_NAME "gresource.xml")
set(GRESOURCE_C_NAME "gresource.c")

set(GRESOURCE_DIR "${CMAKE_SOURCE_DIR}/${GRESOURCE_DIR_NAME}")
set(GRESOURCE_BINARY_DIR "${CMAKE_BINARY_DIR}/${GRESOURCE_DIR_NAME}")
set(GRESOURCE_XML "${GRESOURCE_BINARY_DIR}/${GRESOURCE_XML_NAME}")
set(GRESOURCE_C "${GRESOURCE_BINARY_DIR}/${GRESOURCE_C_NAME}")

file(GLOB_RECURSE RESOURCES "${GRESOURCE_DIR}/*")

set(NEW_CONTENT "")
foreach(RESOURCE ${RESOURCES})
    get_filename_component(RELATIVE_PATH ${RESOURCE} NAME)
    string(REPLACE "${GRESOURCE_DIR}/" "" RELATIVE_PATH "${RESOURCE}")
    if (NOT ${RELATIVE_PATH} STREQUAL "resource.c")
        set(NEW_CONTENT "${NEW_CONTENT}<file>${RELATIVE_PATH}</file>")
    endif()
endforeach()

if(EXISTS ${GRESOURCE_XML})
    file(READ ${GRESOURCE_XML} CURRENT_CONTENT)
else()
    set(CURRENT_CONTENT "")
endif()

if(NOT CURRENT_CONTENT STREQUAL "<gresources><gresource>${NEW_CONTENT}</gresource></gresources>")
    message(STATUS "Generating gresource.xml...")

    if(EXISTS ${GRESOURCE_BINARY_DIR})
        file(REMOVE_RECURSE ${GRESOURCE_BINARY_DIR})
    endif()
    file(MAKE_DIRECTORY ${GRESOURCE_BINARY_DIR})

    file(WRITE ${GRESOURCE_XML} "<gresources><gresource>")
    file(APPEND ${GRESOURCE_XML} "${NEW_CONTENT}")
    file(APPEND ${GRESOURCE_XML} "</gresource></gresources>")
endif()

message(STATUS "Copying current gresources...")
file(COPY ${GRESOURCE_DIR} DESTINATION "${GRESOURCE_BINARY_DIR}/..")

add_custom_command(
    OUTPUT ${GRESOURCE_C}
    COMMAND ${GLIB_COMPILE_RESOURCES} --sourcedir="${GRESOURCE_DIR_NAME}" --generate-source --target="${GRESOURCE_DIR_NAME}/${GRESOURCE_C_NAME}" "${GRESOURCE_DIR_NAME}/${GRESOURCE_XML_NAME}"
    DEPENDS ${GRESOURCE_XML} ${RESOURCES}
    COMMENT "Generating gresource.c from gresource.xml"
)

add_custom_target(GenerateResources DEPENDS ${GRESOURCE_C})

file(GLOB_RECURSE SOURCES "src/*.c")
list(APPEND SOURCES ${GRESOURCE_C})

add_executable(${PROJECT_NAME} ${SOURCES})

if(WIN32)
    add_executable("${PROJECT_NAME}-console" ${SOURCES})
    target_link_libraries(${PROJECT_NAME} ${GTK_LIBRARIES} -mwindows)
    target_link_libraries("${PROJECT_NAME}-console" ${GTK_LIBRARIES})
else()
    target_link_libraries(${PROJECT_NAME} ${GTK_LIBRARIES})
endif()
