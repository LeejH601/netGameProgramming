#include "..\Common.h"

int main(int argc, char* argv[])
{
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;
	printf("[�˸�] ���� �ʱ�ȭ ����\n");

	printf("\n");
	printf("wVersion = %d.%d\n", LOBYTE(wsa.wVersion), HIBYTE(wsa.wVersion));
	printf("wHighVersion = %d.%d\n", LOBYTE(wsa.wHighVersion), HIBYTE(wsa.wHighVersion));
	printf("szDescription = %s\n", wsa.szDescription);
	printf("szSystemStatus = %s\n", wsa.szSystemStatus);
	printf("\n");

	SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sock == INVALID_SOCKET)
		err_quit("socket()");
	printf("[�˸�] ���� ����\n");

	if (closesocket(sock) != SOCKET_ERROR)
		printf("[�˸�] ���� ��ȯ\n");

	WSACleanup();
	return 0;
}