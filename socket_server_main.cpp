#include<stdio.h>
#include<WinSock2.h>
#include<Ws2tcpip.h>
#pragma comment (lib,"ws2_32.lib")

#define IP "172.16.10.150"
#define PORT 8888
#define BUF_SIZE 1024


int main() {
	WSADATA wsaData;
	WORD wVersionRequested = MAKEWORD(2, 2);
	
	// 初始化 Winsock 库
	int result = WSAStartup(wVersionRequested, &wsaData);
	if (0 != result) {
		printf("WSAStartup failed: %d\n", result);
		return 1;
		}

	// 使用 Winsock 库进行网络编程

	SOCKET serverSocket, clientSocket;
	struct sockaddr_in serverAddr, clientAddr;
	char buffer[BUF_SIZE];
	
	// 创建服务器套接字
	serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (INVALID_SOCKET == serverSocket) {
		printf("无法创建套接字\n");
		return 1;
	}

	// 设置服务器地址和端口
	memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	inet_pton(AF_INET,IP, &serverAddr.sin_addr.s_addr);
	serverAddr.sin_port = htons(PORT);

	// 绑定套接字到指定的地址和端口
	if (SOCKET_ERROR == bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)))
	{
		printf("绑定失败\n");
		return 1;
	}

	// 监听传入的连接
	if (SOCKET_ERROR == listen(serverSocket, 1)) {
		printf("监听失败");
		return 1;
	}

	printf("服务器正在监听端口： %d\n", PORT);

	// 接受传入的连接
	int clientAddressSize = sizeof(clientAddr);
	clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientAddressSize);
	if (INVALID_SOCKET == clientSocket) {
		printf("接收连接失败\n");
		return 1;
	}

	printf("已与客户端建立连接\n");

	while (1) {
		// 接收来自客户端的数据
		memset(buffer, 0, sizeof(buffer));
		int recvLen = recv(clientSocket, buffer, BUF_SIZE, 0);
		if (SOCKET_ERROR == recvLen) {
			printf("接收数据失败\n");
			return 1;
		}

		buffer[recvLen] = '\0';
		printf("从客户端收到的数据： %s\n", buffer);
		if (strcmp(buffer, "exit") == 0) break;
		
		if (SOCKET_ERROR == send(clientSocket, buffer, (int)strlen(buffer), 0)) {
			printf("发送响应失败\n");
			return 1;
		}
	}

	// 关闭连接
	// 释放 Winsock 资源
	closesocket(clientSocket);
	closesocket(serverSocket);
	WSACleanup();
	return 0;
}



