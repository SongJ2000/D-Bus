/**
 * Dbus服务端
 *
 * 接收客户端调用方法（method call），执行对应的回调函数（method callback）
 *
 * */

#include <iostream>
#include "DBusCommInterface.h"

GMainLoop *pLoop = nullptr;			// 主循环指针
ComGdbusDemo *pSkeleton = nullptr;	// DBus通信框架指针
pthread_t *pThread = nullptr;		// 线程指针


/* 与总线建立连接成功 回调函数 */
void GBusAcquiredCallback(GDBusConnection *connection, const gchar *name, gpointer userData);
/* 获取总线名称成功 回调函数 */
void GBusNameAcquiredCallback (GDBusConnection *connection, const gchar *name, gpointer userData);
/* 与总线建立连接失败 回调函数 */
void GBusNameLostCallback (GDBusConnection *connection, const gchar *name, gpointer userData);

/* set string方法回调函数 */
gboolean methondSetStringCallback(ComGdbusDemo *object, GDBusMethodInvocation *invocation, const gchar *input, gpointer userData);
/* set double方法回调函数 */
gboolean methondSetDoubleCallback(ComGdbusDemo *object, GDBusMethodInvocation *invocation, gdouble input, gpointer userData);


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

		//// 创建主循环失败，程序结束 ////
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
	if (busID != 0)
	{
		/* 创建线程 */
		pthread_create(pThread, nullptr, threadFunction, nullptr);
		if (pThread == nullptr)
		{
			std::cout << "Error: Failed to create thread." << std::endl;

			//// 创建线程失败，程序结束 ////
			return false;
		}
		/* 等待线程结束 */
		pthread_join(pThread);

		//// 程序正常结束 ////
		return true;
	}
	else
	{
		std::cout << "Error: Failed to connect to dbus." << std::endl;

		//// 连接DBus失败，程序结束 ////
		return false;
	}
}



/* 与总线建立连接成功 回调函数 */
void GBusAcquiredCallback(GDBusConnection *connection, const gchar *name, gpointer userData)
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
void GBusNameAcquiredCallback (GDBusConnection *connection, const gchar *name, gpointer userData)
{
	std::cout << "GBusNameAcquiredCallback: Acquired bus name: " << name << std::endl;
}


/* 与总线建立连接失败 回调函数 */
void GBusNameLostCallback (GDBusConnection *connection, const gchar *name, gpointer userData)
{
	if (connection == nullptr)	std::cout << "GBusNameLostCallback: Failed to connect to dbus." << std::endl;
	else						std::cout << "GBusNameLostCallback: Failed to get dbus name." << std::endl;
	g_main_loop_quit(pLoop);
}


/* set string方法回调函数 */
gboolean methondSetStringCallback(ComGdbusDemo *object, GDBusMethodInvocation *invocation, const gchar *input, gpointer userData)
{
	std::cout << "Received string: " << input << std::endl;
	gdbus_demo_complete_method_set_string(object, invocation, input);
	return true;
}


/* set double方法回调函数 */
gboolean methondSetDoubleCallback(ComGdbusDemo *object, GDBusMethodInvocation *invocation, gdouble input, gpointer userData)
{
	std::cout << "Received value(double): " << input << std::endl;
	gdbus_demo_complete_method_set_double(object, invocation, input);
	return true;
}


/* 用户线程 */
void *threadFunction(void* arg);
{
	/* 启动主循环 */
	g_main_loop_run(pLoop);
	while (true)
	{
		std::cout << "Running..." << std::endl;
		sleep(1);
	}
}