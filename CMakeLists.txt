cmake_minimum_required(VERSION 3.10)

project (GuessHowMuch VERSION 1.0)

set(CMAKE_AUTOMOC ON)

set(Qt5_DIR ${QTDIR})
find_package(Qt5 COMPONENTS Core WebSockets REQUIRED)

include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${Qt5Widgets_INCLUDES})

add_definitions(${Qt5Widgets_DEFINITIONS})

set(CMAKE_CXX_FLAGS "${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")

add_subdirectory("src")

add_executable(server ${SERVER_SOURCE} ${LIB_SOURCE})
add_executable(client ${CLIENT_SOURCE} ${LIB_SOURCE})

target_link_libraries(server Qt5::Core Qt5::WebSockets)
target_link_libraries(client Qt5::Core Qt5::WebSockets)

