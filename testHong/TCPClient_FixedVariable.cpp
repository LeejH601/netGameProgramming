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

	// ���� �ʱ�ȭ
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	// ���� ����
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
		printf("%s �������� �ʴ� ���ϸ��Դϴ�.", filename);
		return 0;
	}

	// ���� ũ�� ���ϱ�
	fseek(fp, 0, SEEK_END);
	int Filesize = ftell(fp);

	fseek(fp, 0, SEEK_SET); // ���� �����͸� ������ ó������ �̵���Ŵ

	// ���� �̸� ���ϱ�
	int NameSize = strlen(filename);

	FileDataBuf = new char[Filesize];
	FileNameBuf = new char[NameSize];

	memset(FileDataBuf, 0, Filesize); // ���� ũ�⸸ŭ �޸𸮸� 0���� �ʱ�ȭ


	// ������ ������ ���
	// ���� �̸� ������ 
	strncpy(FileNameBuf, filename, NameSize);
	// ������ ������(���� �̸� ũ��)
	retval = send(sock, (const char*)&NameSize, sizeof(NameSize), 0);
	if (retval == SOCKET_ERROR) {
		err_display("send() - file name size");
	}

	// ������ ������(�̸� ������) // ũ��
	retval = send(sock, FileNameBuf, NameSize, 0);
	if (retval == SOCKET_ERROR) {
		err_display("send() - file name buf");
	}

	// ������ ������(������) // ���� ũ��
	retval = send(sock, (const char*)&Filesize, sizeof(Filesize), 0);
	if (retval == SOCKET_ERROR) {
		err_display("send() - file size");
	}


	char dataBuf[BUFSIZE];
	int leftDataSize = Filesize; // �� ���� ������
	int bufSize = BUFSIZE;

	while (leftDataSize > 0) {
		if (leftDataSize < BUFSIZE)
			bufSize = leftDataSize;
		else
			bufSize = BUFSIZE;

		// ������ ������ (���� ����) - ���� ����
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
	// ���� �ݱ�
	closesocket(sock);

	// ���� ����
	WSACleanup();
	return 0;
}
