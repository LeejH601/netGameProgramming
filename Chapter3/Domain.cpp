#include "..\Common.h"
#include <iostream>


int main(int argc, char* argv[])
{
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;
	std::string domainInput;
	if (argc >= 1)
		domainInput = argv[1];
	else {
		printf("올바르지 않은 주소\n");
		return 1;
	}

	struct hostent* ipv4hostent = gethostbyname(domainInput.c_str());

	if (ipv4hostent == nullptr) {
		printf("도메인을 찾을 수 없음 \n");
	}
	char* hostaliases = *ipv4hostent->h_aliases;
	int hostalsIndex = 0;
	while (hostaliases != NULL)
	{
		printf("호스트 별명 [%d] : %s\n", hostalsIndex, hostaliases);
		hostaliases += strlen(hostaliases);
		if (*hostaliases == '\0') break;
		++hostalsIndex;
	}

	int hostIPIndex = 0;
	while (ipv4hostent->h_addr_list[hostIPIndex])
	{
		struct in_addr addr;
		memcpy(&addr, ipv4hostent->h_addr_list[hostIPIndex], INET_ADDRSTRLEN);
		char ipv4str[INET_ADDRSTRLEN];
		inet_ntop(AF_INET, (struct in_addr*)(ipv4hostent->h_addr_list[hostIPIndex]), ipv4str, INET_ADDRSTRLEN);
		printf("호스트 IP주소 [%d] : %s\n", hostIPIndex, ipv4str);
		++hostIPIndex;
	}
	return 0;
}
