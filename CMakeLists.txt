cmake_minimum_required(VERSION 3.10)
project(gtk-glade-test)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK REQUIRED gtk+-3.0)

find_program(GLIB_COMPILE_RESOURCES NAMES glib-compile-resources REQUIRED)

include_directories(${GTK_INCLUDE_DIRS})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

set(RESOURCE_DIR "${CMAKE_SOURCE_DIR}/src/res")
set(RESOURCE_BINARY_DIR "${CMAKE_BINARY_DIR}/src/res")
set(RESOURCE_XML "${RESOURCE_BINARY_DIR}/resource.xml")
set(RESOURCE_C "${RESOURCE_BINARY_DIR}/resource.c")

file(GLOB_RECURSE RESOURCES "${RESOURCE_DIR}/*")

set(NEW_CONTENT "")
foreach(RESOURCE ${RESOURCES})
    get_filename_component(RELATIVE_PATH ${RESOURCE} NAME)
    string(REPLACE "${RESOURCE_DIR}/" "" RELATIVE_PATH "${RESOURCE}")
    if (NOT ${RELATIVE_PATH} STREQUAL "resource.c")
        set(NEW_CONTENT "${NEW_CONTENT}<file>${RELATIVE_PATH}</file>")
    endif()
endforeach()

if(EXISTS ${RESOURCE_XML})
    file(READ ${RESOURCE_XML} CURRENT_CONTENT)
else()
    set(CURRENT_CONTENT "")
endif()

if(NOT CURRENT_CONTENT STREQUAL "<gresources><gresource>${NEW_CONTENT}</gresource></gresources>")
    message(STATUS "Generating resource.xml...")

    if(EXISTS ${RESOURCE_BINARY_DIR})
        file(REMOVE_RECURSE ${RESOURCE_BINARY_DIR})
    endif()
    file(MAKE_DIRECTORY ${RESOURCE_BINARY_DIR})

    file(WRITE ${RESOURCE_XML} "<gresources><gresource>")
    file(APPEND ${RESOURCE_XML} "${NEW_CONTENT}")
    file(APPEND ${RESOURCE_XML} "</gresource></gresources>")
endif()

message(STATUS "Copying current resources...")
file(COPY ${RESOURCE_DIR} DESTINATION "${RESOURCE_BINARY_DIR}/..")

add_custom_command(
    OUTPUT ${RESOURCE_C}
    COMMAND ${GLIB_COMPILE_RESOURCES} --sourcedir=${RESOURCE_BINARY_DIR} --generate-source --target=${RESOURCE_C} ${RESOURCE_XML}
    DEPENDS ${RESOURCE_XML} ${RESOURCES}
    COMMENT "Generating resource.c from resource.xml"
)

add_custom_target(GenerateResources DEPENDS ${RESOURCE_C})

file(GLOB SOURCES "src/*.c")
list(APPEND SOURCES ${RESOURCE_C})

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} ${GTK_LIBRARIES})
