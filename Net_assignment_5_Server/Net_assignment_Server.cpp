#include "..\Common.h"
#include <fstream>
#include <filesystem>
#include <string>

char* SERVERIP = (char*)"127.0.0.1";
#define SERVERPORT 8999
#define BUFSIZE 4096

int main(int argc, char* argv[])
{
	int retval;
	std::string root_path;
	
	if (argc > 1) {
		root_path = "C:\\testFolder\\";
	}
	

	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_sock == INVALID_SOCKET) err_quit("socket()");

	struct sockaddr_in serveraddr;
	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(SERVERPORT);
	retval = bind(listen_sock, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit("bind()");

	// listen()
	retval = listen(listen_sock, SOMAXCONN);
	if (retval == SOCKET_ERROR) err_quit("listen()");

	SOCKET client_sock;
	struct sockaddr_in clientaddr;
	int addrlen;
	int len;
	char buf[BUFSIZE + 1];

	std::string file_name;
	std::ofstream recv_file;

	struct file_metaData {
		int nfilename;
		long nfilesize;
	};

	while (true)
	{
		addrlen = sizeof(clientaddr);
		client_sock = accept(listen_sock, (struct sockaddr*)&clientaddr, &addrlen);
		if (client_sock == INVALID_SOCKET) {
			err_display("accept()");
			break;
		}

		char addr[INET_ADDRSTRLEN];
		inet_ntop(AF_INET, &clientaddr.sin_addr, addr, sizeof(addr));
		printf("\n[TCP 서버] 클라이언트 접속: IP 주소=%s, 포트 번호=%d\n", addr, ntohs(clientaddr.sin_port));

		file_metaData meta_data;

		retval = recv(client_sock, (char*)&meta_data, sizeof(meta_data), MSG_WAITALL); // file_info len recv
		if (retval == SOCKET_ERROR) {
			err_display("recv()");
			break;
		}
		else if (retval == 0)
			break;

		file_name.resize(meta_data.nfilename); // string 공간 할당

		retval = recv(client_sock, buf, meta_data.nfilename, MSG_WAITALL); // file_info recv
		if (retval == SOCKET_ERROR) {
			err_display("recv()");
			break;
		}
		else if (retval == 0)
			break;
		printf("%d \n", retval);
		memcpy(file_name.data(), buf, file_name.length());

		printf("[TCP 서버] 파일 이름 : %s\n", file_name.data());
		printf("[TCP 서버] 파일 크기 : %d\n", meta_data.nfilesize);

		std::string file_path = root_path.c_str();

		file_path += file_name.data();
		printf("[TCP 서버] 저장 경로 : %s\n", file_path.data());
		recv_file.open(file_path.c_str(), std::ios_base::binary);

		CONSOLE_SCREEN_BUFFER_INFO cs_Info;
		GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cs_Info);

		long current_file_recv = 0;

		while (true)
		{
			SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cs_Info.dwCursorPosition);

			/*retval = recv(client_sock, (char*)&len, sizeof(len), MSG_WAITALL);
			if (retval == SOCKET_ERROR) {
				err_display("recv()");
				break;
			}
			else if (retval == 0)
				break;*/

			retval = recv(client_sock, buf, BUFSIZE, MSG_WAITALL);
			if (retval == SOCKET_ERROR) {
				err_display("recv()");
				break;
			}
			else if (retval == 0)
				break;

			recv_file.write(buf, retval);
			current_file_recv += retval;
			printf("수신율 : %.2f%%", 100.0f * (float(current_file_recv) / float(meta_data.nfilesize)));
		}

		printf("수신율 : %.2f%%\n", 100.0f * (float(current_file_recv) / float(meta_data.nfilesize)));
		printf("[TCP 서버] 수신완료\n");
		recv_file.close();
		closesocket(client_sock);
		printf("[TCP 서버] 클라이언트 종료: IP 주소=%s, 포트 번호=%d\n", addr, ntohs(clientaddr.sin_port));
	}

	closesocket(listen_sock);

	WSACleanup();
	return 0;
}