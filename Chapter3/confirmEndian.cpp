#include "..\Common.h"

bool IsLittleEndian();
bool IsBigEndian();

bool IsLittleEndian() {
	u_short testBits = 1;
	return (bool)(((char*)&testBits)[0]);
}

bool IsBigEndian() {
	u_short testBits = 1;
	return !(bool)(((char*)&testBits)[0]);
}

int main(int argc, char* argv[]) {
	printf("isLittle?	");
	if (IsLittleEndian()) printf("True");
	else printf("False");
	printf("\n");
	printf("isBig?		");
	if (IsBigEndian()) printf("True");
	else printf("False");
	printf("\n");
}