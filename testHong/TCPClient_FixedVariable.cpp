#include "..\Common.h"

char *SERVERIP = (char *)"127.0.0.1";
#define SERVERPORT 9000
#define BUFSIZE    1024

int main(int argc, char *argv[])
{
	int retval;
	char* filename = nullptr;
	if (argc > 2) {
		SERVERIP = argv[1];
		filename = argv[2];
	}
	else
		return 1;

	FILE* fp = NULL;

	// 윈속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	// 소켓 생성
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) err_quit("socket()");

	// connect()
	struct sockaddr_in serveraddr;
	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	inet_pton(AF_INET, SERVERIP, &serveraddr.sin_addr);
	serveraddr.sin_port = htons(SERVERPORT);
	retval = connect(sock, (struct sockaddr *)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit("connect()");


	char* FileDataBuf = NULL;
	char* FileNameBuf = NULL;

	fp = fopen(filename, "r");
	if (fp == NULL) {
		printf("%s 존재하지 않는 파일명입니다.", filename);
		return 0;
	}

	// 파일 크기 구하기
	fseek(fp, 0, SEEK_END);
	int Filesize = ftell(fp);

	fseek(fp, 0, SEEK_SET); // 파일 포인터를 파일의 처음으로 이동시킴

	// 파일 이름 구하기
	int NameSize = strlen(filename);

	FileDataBuf = new char[Filesize];
	FileNameBuf = new char[NameSize];

	memset(FileDataBuf, 0, Filesize); // 파일 크기만큼 메모리를 0으로 초기화


	// 서버와 데이터 통신
	// 파일 이름 데이터 
	strncpy(FileNameBuf, filename, NameSize);
	// 데이터 보내기(파일 이름 크기)
	retval = send(sock, (const char*)&NameSize, sizeof(NameSize), 0);
	if (retval == SOCKET_ERROR) {
		err_display("send() - file name size");
	}

	// 데이터 보내기(이름 데이터) // 크기
	retval = send(sock, FileNameBuf, NameSize, 0);
	if (retval == SOCKET_ERROR) {
		err_display("send() - file name buf");
	}

	// 데이터 보내기(데이터) // 파일 크기
	retval = send(sock, (const char*)&Filesize, sizeof(Filesize), 0);
	if (retval == SOCKET_ERROR) {
		err_display("send() - file size");
	}


	char dataBuf[BUFSIZE];
	int leftDataSize = Filesize; // 미 수신 데이터
	int bufSize = BUFSIZE;

	while (leftDataSize > 0) {
		if (leftDataSize < BUFSIZE)
			bufSize = leftDataSize;
		else
			bufSize = BUFSIZE;

		// 데이터 보내기 (가변 길이) - 파일 내용
		fread(dataBuf, bufSize, 1, fp);

		retval = send(sock, dataBuf, bufSize, 0);
		if (retval == SOCKET_ERROR) {
			err_display("send() file data ");
			break;
		}

		leftDataSize -= bufSize;
	}

	delete[] FileDataBuf;
	delete[] FileNameBuf;
	fclose(fp);
	// 소켓 닫기
	closesocket(sock);

	// 윈속 종료
	WSACleanup();
	return 0;
}
