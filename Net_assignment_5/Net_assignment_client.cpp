#include "..\Common.h"
#include <fstream>
#include <string>
#include <array>
#include <filesystem>

char* SERVERIP = (char*)"127.0.0.1";
char* SERVERPORT = (char*)"9000";
#define BUFSIZE 4096

int main(int argc, char* argv[])
{
	int retval;
	char* file_path = argv[3];
	std::ifstream send_file;

	if (argc > 3) {
		SERVERIP = argv[1];
		printf("[TCP 클라이언트] 서버 IP 주소 : %s\n", SERVERIP);
		SERVERPORT = argv[2];
		send_file.open(argv[3], std::ios_base::binary);
		printf("[TCP 클라이언트] 파일 이름 : %s\n", file_path);
		/*if (!send_file)
			return 1;*/
	}
	else {
		printf("잘못된 입력\n");
		return 1;
	}

	auto remain_file_size = std::filesystem::file_size(file_path);

	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) err_quit("socket()");

	struct sockaddr_in serveraddr;
	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	inet_pton(AF_INET, SERVERIP, &serveraddr.sin_addr);
	serveraddr.sin_port = htons(atoi(SERVERPORT));
	retval = connect(sock, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit("connect()");

	char buf[BUFSIZE];
	int len;
	std::string filename = std::filesystem::path(file_path).filename().string();

	struct file_metaData {
		int nfilename;
		long nfilesize;
	};

	file_metaData meta_data;
	meta_data.nfilename = filename.length();
	meta_data.nfilesize = remain_file_size;

	retval = send(sock, (char*)&meta_data, sizeof(meta_data), 0);
	if (retval == SOCKET_ERROR) {
		err_display("send()");
	}

	len = meta_data.nfilename;
	retval = send(sock, filename.data(), len, 0);
	if (retval == SOCKET_ERROR) {
		err_display("send()");
	}

	CONSOLE_SCREEN_BUFFER_INFO cs_Info;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cs_Info);

	/*retval = send(sock, (char*)&len, sizeof(len), 0);
	if (retval == SOCKET_ERROR) {
		err_display("send()");
		break;
	}*/

	while (remain_file_size > 0)
	{
		if (remain_file_size > BUFSIZE) {
			send_file.read(buf, BUFSIZE);
			len = BUFSIZE;
		}
		else {
			send_file.read(buf, remain_file_size);
			len = remain_file_size;
		}

		/*retval = send(sock, (char*)&len, sizeof(len), 0);
		if (retval == SOCKET_ERROR) {
			err_display("send()");
			break;
		}*/

		retval = send(sock, buf, len, 0);
		if (retval == SOCKET_ERROR) {
			err_display("send()");
			break;
		}

		remain_file_size -= retval;
		printf("송신율 : %.2f%%", 100.0f * (float(meta_data.nfilesize - remain_file_size) / float(meta_data.nfilesize)));
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cs_Info.dwCursorPosition);
	}
	printf("송신율 : %.2f%%\n", 100.0f * (float(meta_data.nfilesize - remain_file_size) / float(meta_data.nfilesize)));
	printf("[TCP 클라이언트] 송신완료\n");
	closesocket(sock);

	WSACleanup();
	return 0;
}