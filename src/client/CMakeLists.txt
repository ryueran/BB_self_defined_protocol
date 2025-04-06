include_directories(
    ../Message
    include/
)

# Find thread package
find_package(Threads REQUIRED)

add_executable(client src/client.cc)

target_link_libraries(client PRIVATE Threads::Threads)