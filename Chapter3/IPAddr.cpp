//#include "..\Common.h"
//
//int main(int argc, char* argv[])
//{
//	WSADATA wsa;
//	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
//		return 1;
//
//	const char* ipv4test = "147.46.114.70";
//	printf("IPv4 주소(변환 전) =  %s\n", ipv4test);
//
//	struct in_addr ipv4num;
//	inet_pton(AF_INET, ipv4test, &ipv4num);
//	printf("IPv4 주소(변환 후) = %#x\n", ipv4num.s_addr);
//
//	char ipv4str[INET_ADDRSTRLEN];
//	inet_ntop(AF_INET, &ipv4num, ipv4str, sizeof(ipv4str));
//	printf("IPv4 주소(다시 변환 후) = %s\n", ipv4str);
//
//
//	const char* ipv6test = "147.46.114.70";
//	printf("IPv6 주소(변환 전) =  %s\n", ipv6test);
//
//	struct in6_addr ipv6num;
//	inet_pton(AF_INET6, ipv6test, &ipv6num);
//	printf("IPv6 주소(변환 후) = 0x");
//
//	char ipv4str[INET_ADDRSTRLEN];
//	inet_ntop(AF_INET6, &ipv4num, ipv4str, sizeof(ipv4str));
//	printf("IPv4 주소(다시 변환 후) = %s\n", ipv4str);
//
//
//	WSACleanup();
//	return 0;
//}