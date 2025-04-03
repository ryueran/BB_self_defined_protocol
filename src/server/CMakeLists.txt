# Include directories
include_directories(
    include
    ../Message
)

set(SRC_FILES
    src/main_server.cc        # Entry point
    src/Acceptor.cc
    src/Epoller.cc
    src/Handler.cc
    src/Reactor.cc
    src/Server.cc
)

add_executable(server ${SRC_FILES})
