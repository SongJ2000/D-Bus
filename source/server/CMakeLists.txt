cmake_minimum_required(version 3.10)

project(server)

include_directories(../../include)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ../../bin)

add_executable(server server.cpp ../interface/GDBusInterface.cpp)

target_link_libraries(server pthread gio-2.0 gobject-2.0 gmodule-2.0 gthread-2.0 glib-2.0)