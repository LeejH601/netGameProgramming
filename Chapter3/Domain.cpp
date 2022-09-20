#include "..\Common.h"
#include <iostream>
#include <Windows.h>


int main()
{
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	while (true)
	{
		std::string domainInput;
		std::cin >> domainInput;

		struct hostent* ipv4hostent = gethostbyname(domainInput.c_str());

		if (ipv4hostent == nullptr) {
			printf("도메인을 찾을 수 없음 \n");
			continue;
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
			inet_ntop(AF_INET,  (struct in_addr*)(ipv4hostent->h_addr_list[hostIPIndex]), ipv4str, INET_ADDRSTRLEN);
			printf("호스트 IP주소 [%d] : %s\n", hostIPIndex, ipv4str);
			/*char str[50] = { "tracert " };
			system(strcat(str, ipv4str));*/
			++hostIPIndex;
		}
		printf("\n");
		char str[50] = { "nslookup " };
		system(strcat(str, domainInput.c_str()));
	}
	return 0;
}
