# D-Bus
使用GDBus框架，GDBus是一种基于d-bus技术的进程间通信框架，需要GIO库（https://docs.gtk.org/gio/）。
首先需要定义通信接口，GDBus通信接口是在xml文件下描述的，通过gdbus-codegen工具就可以自动生成对应D-Bus interface源文件和头文件。
