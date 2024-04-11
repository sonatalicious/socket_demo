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
	
	// ��ʼ�� Winsock ��
	int result = WSAStartup(wVersionRequested, &wsaData);
	if (0 != result) {
		printf("WSAStartup failed: %d\n", result);
		return 1;
		}

	// ʹ�� Winsock �����������

	SOCKET serverSocket, clientSocket;
	struct sockaddr_in serverAddr, clientAddr;
	char buffer[BUF_SIZE];
	
	// �����������׽���
	serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (INVALID_SOCKET == serverSocket) {
		printf("�޷������׽���\n");
		return 1;
	}

	// ���÷�������ַ�Ͷ˿�
	memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	inet_pton(AF_INET,IP, &serverAddr.sin_addr.s_addr);
	serverAddr.sin_port = htons(PORT);

	// ���׽��ֵ�ָ���ĵ�ַ�Ͷ˿�
	if (SOCKET_ERROR == bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)))
	{
		printf("��ʧ��\n");
		return 1;
	}

	// �������������
	if (SOCKET_ERROR == listen(serverSocket, 1)) {
		printf("����ʧ��");
		return 1;
	}

	printf("���������ڼ����˿ڣ� %d\n", PORT);

	// ���ܴ��������
	int clientAddressSize = sizeof(clientAddr);
	clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientAddressSize);
	if (INVALID_SOCKET == clientSocket) {
		printf("��������ʧ��\n");
		return 1;
	}

	printf("����ͻ��˽�������\n");

	while (1) {
		// �������Կͻ��˵�����
		memset(buffer, 0, sizeof(buffer));
		int recvLen = recv(clientSocket, buffer, BUF_SIZE, 0);
		if (SOCKET_ERROR == recvLen) {
			printf("��������ʧ��\n");
			return 1;
		}

		buffer[recvLen] = '\0';
		printf("�ӿͻ����յ������ݣ� %s\n", buffer);
		if (strcmp(buffer, "exit") == 0) break;
		
		if (SOCKET_ERROR == send(clientSocket, buffer, (int)strlen(buffer), 0)) {
			printf("������Ӧʧ��\n");
			return 1;
		}
	}

	// �ر�����
	// �ͷ� Winsock ��Դ
	closesocket(clientSocket);
	closesocket(serverSocket);
	WSACleanup();
	return 0;
}



