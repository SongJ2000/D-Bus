# D-Bus/GDbus
使用GDBus框架，GDBus是一种基于D-Bus技术的进程间通信框架，需要GIO库（https://docs.gtk.org/gio/）支持。

## 概述
此demo由三部分组成：GDbus接口、封装接口、测试程序，其中测试程序分为server clinet。

### GDbus接口
GDbus接口是用户根据一定规则自定义的通信接口，其描述在xml文件中，包括方法（method）和信号（signal），通过**gdbus-codegen**工具就可以根据用户编写的xml文件生成接口文件（.c .h）。

命令：`gdbus-codegen --generate-c-code=GDbusInterface interface.xml`，根据interface.xml生成GDbusInterface.c GDbusInterface.h

**GDbusInterface.xml：**
``` xml
<?xml version="1.0" encoding="utf-8"?>
<node>
  <interface name="gdbus.demo">
    
    <method name="methodSetString">
      <arg name="input" type="s" direction="in"/>
      <arg name="response" type="s" direction="out"/>
    </method>
    
    <method name="methodSetDouble">
      <arg name="input" type="d" direction="in"/>
      <arg name="response" type="d" direction="out"/>
    </method>
    
    <signal name="signalSendString">
      <arg name="input" type="s"/>
    </signal>
    
    <signal name="signalSendDouble">
      <arg name="input" type="d"/>
    </signal>
    
  </interface>
</node>
```







