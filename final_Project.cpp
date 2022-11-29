#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#pragma warning (disable : 4996)

uint8_t namelengcheck(uint8_t* name);
uint32_t getseed(uint8_t* name);
uint8_t getrand();

uint32_t seed = 0;

int main() {

	uint8_t name[5] = { 0, };
	printf("Hello\n");
	scanf("%s", name);

	seed = getseed(name);

	getrand();
	printf("%s\n", name);
	return 0;
}

// 이름 길이 반환하는 함수
uint8_t namelengcheck(uint8_t * name)
{
	uint32_t i = 0;
	while (name[i++] != '\0');

	return (i - 1) >= 2 ? (i - 1) : 0;
}

// 이름으로 시드값 얻는 함수
uint32_t getseed(uint8_t* name)
{
	size_t length = namelengcheck(name);
	uint32_t retseed = 0;
	if (length) printf("2이상입니다.\n");
	else {
		printf("2 미만입니다.\n");
		return 0;
	}
	for (size_t i = 0; i < length; i++)
	{
		retseed += name[i];
	}
	printf("retseed : %d\n", retseed);
	return retseed;
}
// 난수 얻는 함수
uint8_t getrand()
{
	srand(seed);

	printf("난수 : %d\n", rand() % 7);
	printf("난수 : %d\n", rand() % 7);
	return 0;

}
