add_subdirectory(Application)

set(SERVER_SOURCE
    ${SERVER_SOURCE}
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    PARENT_SCOPE)

set(SERVER_HEADERS
    ${SERVER_HEADERS}
    PARENT_SCOPE)