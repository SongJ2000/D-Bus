/**
 * Dbus服务端
 *
 * 接收客户端调用方法（method call），执行对应的回调函数（method callback）
 *
 * */

#include <iostream>
#include "DBusCommInterface.h"

GMainLoop *pLoop = nullptr;			// 主循环指针
ComGdbusDemo *pSkeleton = nullptr;	// 通信框架指针
pthread_t tID = nullptr;


/* 与总线建立连接成功 回调函数 */
void GBusAcquiredCallback(GDBusConnection *connection, const gchar *name, gpointer user_data);
/* 获取总线名称成功 回调函数 */
void GBusNameAcquiredCallback (GDBusConnection *connection, const gchar *name, gpointer user_data);
/* 与总线建立连接失败 回调函数 */
void GBusNameLostCallback (GDBusConnection *connection, const gchar *name, gpointer user_data);

/* set string方法回调函数 */
gboolean methondSetStringCallback(ComGdbusDemo *object, GDBusMethodInvocation *invocation, const gchar *input, gpointer user_data);
/* set double方法回调函数 */
gboolean methondSetDoubleCallback(ComGdbusDemo *object, GDBusMethodInvocation *invocation, gdouble input, gpointer user_data);


/* 用户线程 */
void *threadFunction(void* arg);


bool main()
{
	std::cout << "Server start!" << std::endl;

	/* 创建主循环 */
	pLoop = g_main_loop_new(nullptr, false);
	if (pLoop == nullptr)
	{
		std::cout << "Error: Can't create main loop!" << std::endl;
		return false
	}

	/* 与总线建立连接 */
	guint busID = g_bus_own_name (COM_GDBUS_DEMO_BUS,
	                              COM_GDBUS_DEMO_NAME,
	                              G_BUS_NAME_OWNER_FLAGS_NONE,
	                              &GBusAcquiredCallback,
	                              &GBusNameAcquiredCallback,
	                              &GBusNameLostCallback,
	                              nullptr,
	                              nullptr);
	/* 创建线程 */
	pthread_create(&tID, nullptr, threadFunction, nullptr);
	/* 等待线程结束 */
	pthread_join()
}



/* 与总线建立连接成功 回调函数 */
void GBusAcquiredCallback(GDBusConnection *connection, const gchar *name, gpointer user_data)
{
	GError *pError = nullptr;

	/* 创建skeleton对象 */
	pSkeleton = com_gdbus_demo_skeleton_new();
	g_dbus_interface_skeleton_export(G_DBUS_INTERFACE_SKELETON(pSkeleton), connection, COM_GDBUS_DEMO_OBJECT_PATH, &pError);

	if (pError == nullptr)
	{
		std::cout << "GBusAcquiredCallback: Create skeleton successfully." << std::endl;
	}
	else
	{
		std::cout << "GBusAcquiredCallback: Error: Failed to create skeleton. Reason: " << pError->message << '.' << std::endl;
		g_error_free(pError);
		g_main_loop_quit(pLoop);
	}

	/* 注册set string方法回调函数 */
	g_signal_connect(pSkeleton, "handle-method-set-string", G_CALLBACK(methondSetStringCallback), nullptr);
	/* 注册set double方法回调函数 */
	g_signal_connect(pSkeleton, "handle-method-set-double", G_CALLBACK(methondSetDoubleCallback), nullptr);
}


/* 获取总线名称成功 回调函数 */
void GBusNameAcquiredCallback (GDBusConnection *connection, const gchar *name, gpointer user_data)
{
	std::cout << "GBusNameAcquiredCallback: Acquired bus name: " << name << std::endl;
}


/* 与总线建立连接失败 回调函数 */
void GBusNameLostCallback (GDBusConnection *connection, const gchar *name, gpointer user_data)
{
	if (connection == nullptr)	std::cout << "GBusNameLostCallback: Failed to connect to dbus." << std::endl;
	else						std::cout << "GBusNameLostCallback: Failed to get dbus name." << std::endl;
	g_main_loop_quit(pLoop);
}


/* set string方法回调函数 */
gboolean methondSetStringCallback(ComGdbusDemo *object, GDBusMethodInvocation *invocation, const gchar *input, gpointer user_data)
{
	std::cout << "Received string: " << input <<std::endl;
    gdbus_demo_complete_method_set_string(object, invocation, input);
    return TRUE;
}


/* set double方法回调函数 */
gboolean methondSetDoubleCallback(ComGdbusDemo *object, GDBusMethodInvocation *invocation, gdouble input, gpointer user_data)
{
	std::cout << "Received value(double): " << input <<std::endl;
    gdbus_demo_complete_method_set_double(object, invocation, input);
    return TRUE;
}


/* 用户线程 */
void *threadFunction(void* arg);
{
    g_main_loop_run(pLoop);
    std::cout << "Running..." << std::endl;
    sleep(1);
}