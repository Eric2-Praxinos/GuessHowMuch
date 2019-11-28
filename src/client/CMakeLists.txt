add_subdirectory(Application)

set(CLIENT_SOURCE
    ${CLIENT_SOURCE}
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    PARENT_SCOPE)

set(CLIENT_HEADERS
    ${CLIENT_HEADERS}
    PARENT_SCOPE)