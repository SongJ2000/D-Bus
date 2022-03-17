/**
 * Dbus客户端
 *
 * 接收服务端信号（signal），执行对应的回调函数（signal callback）
 * 向服务端发送方法调用请求
 *
 * */

#include <iostream>
#include "DBusCommInterface.h"


GMainLoop *pLoop = nullptr;				// 主循环指针
GDBusConnection *pConnection = nullptr;	// DBus连接指针
ComGdbusDemo *pProxy = nullptr;			// DBus代理指针
pthread_t *pThread = nullptr;			// 线程指针


/* signal send string回调函数 */
gboolean signalSendStringCallback(ComGdbusDemo *object, const gint *arg, gpointer userData);
/* signal send double回调函数 */
gboolean signalSendDoubleCallback(ComGdbusDemo *object, const gint *arg, gpointer userData);


int main()
{
	GError *pConnError = nullptr;
	GError *pProxyError = nullptr;

	/* 创建主循环 */
	pLoop = g_main_loop_new(nullptr, false);
	if (pLoop == nullptr)
	{
		std::cout << "Error: Create main loop failed." << std::endl;

		//// 创建主循环失败，程序结束 ////
		return false
	}
	/* 与DBus建立连接 */
	pConnection = g_bus_get_sync(COM_GDBUS_DEMO_BUS, nullptr, &pConnError);
	if (pConnError == nullptr)
	{
		/* 创建DBus代理对象 */
		pProxy = com_gdbus_demo_proxy_new_sync(pConnection,
		                                       G_DBUS_PROXY_FLAGS_NONE,
		                                       COM_GDBUS_DEMO_NAME,
		                                       COM_GDBUS_DEMO_OBJECT_PATH,
		                                       nullptr,
		                                       &pProxyError);
		if (pProxy == nullptr)
		{
			std::cout << "Create proxy failed. Reason: " << pConnError->message << std::endl;
			g_error_free(pProxyError);
		}
		else
		{
			std::cout << "Create proxy successfully." << std::endl;

			/* 注册send string信号回调函数 */
			g_signal_connect(pProxy, "signal-send-string", G_CALLBACK(signalSendStringCallback), nullptr);
			/* 注册send double信号回调函数 */
			g_signal_connect(pProxy, "signal-send-double", G_CALLBACK(signalSendDoubleCallback), nullptr);
		}
	}
	else
	{
		std::cout << "Failed to connect to dbus. Reason: " << pConnError->message << std::endl;
		g_error_free(pConnError);

		//// 连接DBus失败，程序结束 ////
		return false;
	}
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



/* signal send string回调函数 */
gboolean signalSendStringCallback(ComGdbusDemo *object, const gint *input, gpointer userData)
{
	std::cout << "signalSendStringCallback: Received string: " << input << std::endl;
	return true;
}

/* signal send double回调函数 */
gboolean signalSendDoubleCallback(ComGdbusDemo *object, gdouble input, gpointer userData)
{
	std::cout << "signalSendDoubleCallback: Received value(double): " << input << std::endl;
	return true;
}

void *threadFunction(void *arg)
{
	/* 启动主循环 */
	g_main_loop_run(pLoop);
	while (true)
	{
		std::cout << "Running..." << std::endl;
		sleep(1);
	}
}


