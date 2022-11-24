#include <stdio.h>
#pragma warning (disable : 4996)
int main() {
	/*
	* 입력하는 파일
	FILE* fp;
	fp = fopen("C:\\Users\\dnjsw\\OneDrive\\바탕 화면\\key.file", "wb");
	
	fseek(fp, 0, SEEK_END);
	int fsize = ftell(fp);
	printf("%d", fsize);
	

	for (int i = 0x1; i < 0x100; i+=2)
	{
		fputc(i, fp);
	}
	for (int i = 0x0; i < 0x100; i += 2)
		fputc(i, fp);
	
	fclose(fp);
	*/
	
	FILE* fp = fopen("C:\\Users\\dnjsw\\OneDrive\\바탕 화면\\key.file", "rb");

	char check[0x101] = { 0 };

	fseek(fp, 0, 2);
	int fsize = ftell(fp);
	int idx;
	// file size가 0x100인지 체크
	if (fsize == 0x100)
	{
		fseek(fp, 0, SEEK_SET);
	}
	else {
		printf("FALSE");
		fclose(fp);
		return 0;
	}
	// 0 ~ 0xFF 가 다 들어가있는지 확인
	for (int i = 0; i < 0x100; i++)
	{
		idx =fgetc(fp);
		if (check[idx] == 1) {
			printf("FALSE");
			fclose(fp);
			return 0;
		}
		check[idx] = 1;
	}
	printf("TRUE");
	fclose(fp);
	return 0;
}
