#include <stdio.h>
#include <stdint.h>
#pragma warning (disable : 4996)

uint8_t name_check(uint8_t* name);

int main() {

	uint8_t name[5] = {0,};
	printf("Hello\n");
	scanf("%s", name);
	if (name_check(name)) printf("2이상입니다.\n");
	else {
		printf("2 미만입니다.\n");
		return 0;
	}
		
	printf("%s\n", name);
	return 0;
}

uint8_t name_check(uint8_t * name)
{
	int i = 0;
	while (name[i++] != '\0');

	return (i - 1) >= 2 ? 1 : 0;
}
