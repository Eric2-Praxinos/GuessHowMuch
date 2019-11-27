add_subdirectory(client)
add_subdirectory(lib)
add_subdirectory(server)

set(CLIENT_SOURCE
    ${CLIENT_SOURCE}
    PARENT_SCOPE)

set(LIB_SOURCE
    ${LIB_SOURCE}
    PARENT_SCOPE)

set(SERVER_SOURCE
    ${SERVER_SOURCE}
    PARENT_SCOPE)
    