#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#pragma warning (disable : 4996)

#define FAIL "fail\n"
#define SUCC "success\n"

// lcg 상수
#define MULT 0x343FD
#define	INCR 0x269EC3

#define BYTETODWORD(x,y,z ) { \
	x |= (y << z); \
	z +=8; \
}
#define RESET(x, y, z) x = 0, y = 0, z =0
/*
	2022-11-30 start

	리버싱 문제 제작

	name값과 키파일을 입력받아

	name값으로 계산 순서를 정하고

	name 값을 연산한다.

	그 값이 keyfile 값과 같으면 통과

*/
// C:\\Users\\leewonjoo\\Desktop\\file.key
uint8_t filename[100] = "C:\\Users\\leewonjoo\\Desktop\\1234.txt";

uint8_t namelengcheck(uint8_t* );
uint8_t filelengcheck(uint8_t* );
uint8_t filecalc(uint8_t*, size_t, size_t);
uint32_t getseed(uint8_t* );
uint8_t lcg();
uint8_t getrand();
uint8_t calc(uint8_t* );
uint8_t byteTobit(uint8_t , size_t, uint8_t*);
uint8_t bitTobyte(uint8_t*, size_t, uint8_t*);

uint32_t seed = 0;						// 이름으로 만든 seed 값
size_t namelength = 0;					// 이름 길이
size_t filelength = 0;					// 파일 길이
uint8_t operators[8] = { 0 };			// 연산 순서 저장
uint8_t operand1[8] = { 0 };			// 연산에 사용될 첫 번째 오퍼랜드
uint8_t operand2[8] = { 0 };			// 연산에 사용될 두 번째 오퍼랜드
uint8_t comres1[8] = { 0 };				// 내부 연산 저장될 변수
uint8_t comres2[8] = { 0 };				// 파일 연산 저장될 변수
uint8_t *filedata;						// 파일 NAND 연산 저장될 변수
size_t f_calc_count = 0;				// 파일 연산 횟수 저장될 변수

int main() {

	uint8_t name[8] = { 0 };

	printf("name : ");
	scanf("%s", name);
	printf("file : ");
	scanf("%s", filename);
	seed = getseed(name);
	if (seed == 0) {
		printf("\n%s\n", FAIL);
		return 0;
	}
	if (!filelengcheck(filename)) {
		return 0;
	}

	filedata = (uint8_t*)malloc(sizeof(uint8_t) * (((namelength * 8) * 4) + f_calc_count + 1));
	getrand();
	calc(name);
	size_t bs = 0;
	for(size_t i =0;i<namelength;i++)
	{
		byteTobit(name[i], 8, (filedata + bs));
		bs += 8;
	}
	if (!filecalc(filename, (namelength * 8), f_calc_count))
	{
		printf("\n%s\n", FAIL);
		system("pause");
		free(filedata);
		return 0;
	}

	for (size_t i = 0; i < namelength; i++)
	{
		bitTobyte(&comres2[i], 8, (filedata+ bs));
		//printf("\ncomres2 : %X", comres2[i]);
		bs += 8;
	}
	for (size_t i = 0; i < namelength; i++)
	{
		//printf("com1 : %X | com2 : %X\n", comres1[i], comres2[i]);
		if (comres1[i] != comres2[i]) {
			printf("\n%s\n", FAIL);
			system("pause");
			free(filedata);
			return 0;
		}
	}
	printf("\n%s\n", SUCC);
	system("pause");
	free(filedata);
	return 0;
}
// 이름 길이 반환하는 함수
uint8_t namelengcheck(uint8_t * name)
{
	uint32_t i = 0;
	while (name[i++] != '\0');
	return (i - 1) >= 2 ? (i - 1) : 0;
}

// 파일 길이를 얻는 함수
uint8_t filelengcheck(uint8_t* fname)
{
	FILE* fp = fopen((char*)fname, "rb");
	if (fp == NULL) {
		printf("없는 파일입니다.\n");
		return 0;
	}
	fseek(fp, 0, SEEK_END);
	filelength = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	//printf("파일 길이는 : %d\n", filelength);
	if ((filelength % 0xC) != 0)
	{
		printf("파일이 0xC의 배수가 아닙니다.\n");
		fclose(fp);
		return 0;
	}
	f_calc_count = filelength / 0xC;
	//printf("f_calc_count : %X\n", f_calc_count);
	fclose(fp);
	return 1;
}

// 파일 내용으로 연산하는 함수
uint8_t filecalc(uint8_t* fname, size_t index, size_t count)
{
	FILE* fp = fopen((char*)fname, "rb");
	if (fp == NULL) {
		return 0;
	}
	//printf("\n");
	size_t first = 0, second = 0, third = 0, digit = 0;
	uint8_t firstbuf[4] = { 0 }, secondbuf[4] = { 0 }, thirdbuf[4] = { 0 };
	while (count--)
	{
		fread(firstbuf, 4, 1, fp);
		for (size_t i = 0; i < 4; i++) 
			BYTETODWORD(first, firstbuf[i], digit);
		
		//printf("%X ", first);
		digit = 0;
		
		fread(secondbuf, 4, 1, fp);
		for (size_t i = 0; i < 4; i++) 
			BYTETODWORD(second, secondbuf[i], digit);
		
		//printf("%X ", second);
		digit = 0;

		fread(thirdbuf, 4, 1, fp);
		for (size_t i = 0; i < 4; i++)
			BYTETODWORD(third, thirdbuf[i], digit);
		
		//printf("%X ", third);
		//printf("index : 0x%02X\n", index);
		if (third < index) // nand 연산에 사용될 주소가 입력 공간에 사용되면 종료
		{
			fclose(fp);
			return 0;
		}
		digit = 0;
		filedata[third] = ~(filedata[first] & filedata[second]);
		//printf("\t\tthird(%x) : %X ~& %X %X\n",third, filedata[first], filedata[second], filedata[third]);
		//printf("\n");
		RESET(first, second, third);
	}
	fclose(fp);
	return 1;
}

// 이름으로 시드값 얻는 함수
uint32_t getseed(uint8_t* name)
{
	namelength = namelengcheck(name);
	uint32_t retseed = 0;
	if (namelength < 2) {
		return 0;
	}
	for (size_t i = 0; i < namelength; i++)
	{
		retseed += name[i];
	}
	//printf("retseed : %d\n", retseed);
	return retseed;
}

// lcg
uint8_t lcg()
{
	seed = seed * MULT + INCR;
	return (seed>>16)%32768;
}

// 난수 얻는 함수
uint8_t getrand()
{
	uint8_t idx = 0;
	// 연산 난수 인풋
	for (size_t i = 0; i < namelength; i++)
	{
		operators[i] = lcg() % 4;
		for (size_t j = 0; j < i; j++)
			if (operators[i] == operators[j]) i--;
		//printf("order : %d\n", operators[i]);
	}
	// 오퍼랜드 1 난수 인풋
	for (size_t i = 0; i < namelength; i++)
	{
		operand1[i] = lcg() % namelength;
		for (size_t j = 0; j < i; j++)
			if (operand1[i] == operand1[j])	i--;
	}
	//for(size_t i =0;i< namelength; i++)
	//	printf("op1[%d] : %d\n", i, operand1[i]);
	// 오퍼랜드 2 난수 인풋
	for (size_t i = 0; i < namelength; i++)
	{
		operand2[i] = lcg() % namelength;
		for (size_t j = 0; j < i; j++)
			if (operand2[i] == operand2[j]) i--;
		
	}
	//for (size_t i = 0; i < namelength; i++)
	//	printf("op2[%d] : %d\n", i, operand2[i]);
	return 0;
}

// 연산 하는 함수
uint8_t calc(uint8_t* name)
{
	//printf("res : ");
	for(size_t i =0;i< namelength;i++)
	{
		uint8_t op1 = operand1[i];
		uint8_t op2 = operand2[i];

		switch (operators[i])
		{
		case 0:
			comres1[i] = name[op1] + name[op2];
			//printf("%X + %X = %X\n", name[op1], name[op2], comres1[i]);
			break;
		case 1:
			comres1[i] = name[op1] - name[op2];
			//printf("%X - %X = %X\n", name[op1], name[op2], comres1[i]);
			break;
		case 2:
			comres1[i] = name[op1] * name[op2];
			//printf("%X * %X = %X\n", name[op1], name[op2], comres1[i]);
			break;
		case 3:
			comres1[i] = name[op1] ^ name[op2];
			//printf("%X ^ %X = %X\n", name[op1], name[op2], comres1[i]);
			break;
		}
	}

	//for (size_t i = 0; i < namelength; i++)
	//	printf("%02X ", comres1[i]);
	return 0;
}

// byte -> bit 변경하는 함수
uint8_t byteTobit(uint8_t byte, size_t length, uint8_t* bit)
{
	//printf("\n");
	while (length--) bit[length] = (byte >> (7 - length)) & 1;
	//for (int i = 0; i < 8; i++) printf("%d", bit[i]);
	//printf("\n");
	return 0;
}

// bit -> byte 변경하는 함수
uint8_t bitTobyte(uint8_t* byte , size_t length, uint8_t* bit)
{
	while (length--) *byte |= bit[length] << (7 - length);
	//printf("%c  = %X\n",*byte, *byte);
	return 0;
}
