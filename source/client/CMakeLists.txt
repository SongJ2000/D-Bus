cmake_minimum_required(version 3.10)

project(client)

include_directories(../../include)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ../../bin)

add_executable(client client.cpp ../interface/GDBusInterface.cpp)

target_link_libraries(client pthread gio-2.0 gobject-2.0 gmodule-2.0 gthread-2.0 glib-2.0)