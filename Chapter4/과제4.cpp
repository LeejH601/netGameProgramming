#include "../Common.h"

char* SERVERIP = (char*)"127.0.0.1";
#define SERVERPORT 9000
#define BUFSIZE 512

int main(int argc, char* argv[])
{
	int retval;
	u_short s_portnum;
	u_short e_portnum;

	if (argc > 3) {
		SERVERIP = argv[1];
		s_portnum = atoi( argv[2]);
		e_portnum = atoi(argv[3]);
	}
	else return 1;

	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	printf("ip 주소 : %s", SERVERIP);
	printf("포트번호 1: %d", s_portnum);
	printf("포트번호 2: %d\n", e_portnum);

	for (u_short i = s_portnum; i <= e_portnum; ++i) {
		SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
		if (sock == INVALID_SOCKET) err_quit("socket()");

		struct sockaddr_in serveraddr;
		memset(&serveraddr, 0, sizeof(serveraddr));
		serveraddr.sin_family = AF_INET;
		inet_pton(AF_INET, SERVERIP, &serveraddr.sin_addr);
		serveraddr.sin_port = htons(i);

		retval = connect(sock, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
		if (retval == SOCKET_ERROR) printf("%d 포트 연결 실패\n", i);
		else printf("%d 포트 연결 성공\n", i);

		closesocket(sock);

	}
	
	WSACleanup();
	return 0;
}