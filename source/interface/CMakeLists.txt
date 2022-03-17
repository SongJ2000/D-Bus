cmake_minimum_required(version 3.10)

project(DBusCommInterface)

set(LIBRARY_OUTPUT_PATH  ../../lib)

include_directories(../../include)

# 编译动态库
add_library(DBusCommInterface SHARED DBusCommInterface.cpp GDBusInterface.c) 

link_directories(DBusCommInterface gio glib gobject)



