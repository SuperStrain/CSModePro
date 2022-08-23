#include <iostream>
#include<string>
#include<WinSock2.h>
#pragma comment(lib,"ws2 32.lib")
using namespace std;

int main(int argc, char *argv[])
{
	WSADATA wsaData;
	WORD libversion = MAKEWORD(2, 2);
	//初始化操作
	if (WSAStartup(libversion, &wsaData) != 0)
	{
		cout << "Init Oprating error.(初始化失败) \n";
		return 0;
	}
	if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2)
	{
		WSACleanup();
		cout << "Can't Find valid lib. \n";
	}
	//创建套接字操作
	SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (serverSocket == INVALID_SOCKET)
	{
		cout << "嵌套字创建失败。\n";
		WSACleanup();
		return 0;
	}
	//绑定套接字
	sockaddr_in sockAddrs;
	sockAddrs.sin_family = AF_INET;
	sockAddrs.sin_port = htons(1234);
	sockAddrs.sin_addr.S_un.S_addr = ADDR_ANY;

	if (bind(serverSocket, (sockaddr*)&sockAddrs, sizeof(sockAddrs)) == SOCKET_ERROR)
	{
		cout << "bind error.\n";  //绑定失败
		closesocket(serverSocket); //关闭套接字
		WSACleanup();
		return 0;
	}

	//监听操作
	if (listen(serverSocket, 30) == SOCKET_ERROR)
	{
		cout << "listen error.\n";
		closesocket(serverSocket);
		WSACleanup();
		return 0;
	}

	//进入监听状态，等待客户端连接
	SOCKET clientSocket;
	sockaddr_in client_sin;
	bool flags = 0;//标志是否有客户端连接

	while (1)
	{
		if (!flags)
		{
			cout << "\n服务器端已经启动，等等客户端连接......\n" << endl;
		}
		int len = sizeof(client_sin);
		clientSocket = accept(serverSocket, (sockaddr*)&client_sin, &len);
		if (clientSocket == INVALID_SOCKET)
		{
			flags = 0;
			cout << "accept error.\n";
			closesocket(serverSocket);
			closesocket(clientSocket);
			WSACleanup();
			return 0;
		}

		if (!flags)
		{
			cout << "服务器收到一个客户端连接：" << inet_ntoa(client_sin.sin_addr) << endl;
			flags = 1;
		}

		//接受数据信息
		char rMsg[512];
		memset(rMsg, 0, 512);//初始化rMgs，防止数组越界
		int recvMsgLen = recv(clientSocket, rMsg, sizeof(rMsg), 0);
		if (recvMsgLen)
		{
			cout << "\n已收到客户端消息：" << rMsg << endl;
		}
		string sendMsg;
		cout << "\n请输入要发送的消息：" << endl;
		//这里采用getline作为输入函数，因为cin函数不能输入空格（输入空格视为终止）
		getline(cin, sendMsg);
		send(clientSocket, sendMsg.c_str(), strlen(sendMsg.c_str()), 0);//发送给客户端

		closesocket(clientSocket);//关闭客户端套接字

	}
	closesocket(serverSocket);//关闭服务器套接字
	WSACleanup();
	return 0;
}


