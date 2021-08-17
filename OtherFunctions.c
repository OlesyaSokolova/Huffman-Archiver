#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "OtherFunctions.h"

unsigned char * BinarySymbolString(unsigned char symbol)
{
	unsigned char result[9] = { '\0' };
	unsigned char r[9] = { '\0' };
	int c, i = 0, lenght, j = 0;
	c = (int)symbol;
	while (c != 0)
	{
		result[i] = c % 2;
		result[i] = result[i] + '0';
		c = c / 2;
		i++;
	}
	lenght = i;
	if (lenght < 8)
	{
		for (j = 0; j < 8 - lenght; j++)
			r[j] = '0';

	}
	for (i = lenght - 1; i >= 0; i--)
	{
		r[j] = result[i];
		j++;
	}
	r[8] = '\0';
	return r;
}
unsigned char  ASCII_BinaryCodeToChar(unsigned char a[9])
{
	int i = 0, k, st, sum = 0, x = 0;
	unsigned char c;
	while (a[i] != '\0')
	{
		i++;
	}
	k = i;
	//r - разряд цифры (k - количество цифр) 
	int count = 0;//номер цифры с начала 
	for (i = k - 1; i >= 0; i--)
	{
		st = pow(2, i);
		x = (a[count] - '0') * st;
		sum += x;
		count++;
	}
	c = sum;
	return c;
}
unsigned char PackingByte(unsigned char eight[9])
{
	unsigned char byte = 0;
	int i;
	for (i = 0; i < 8; i++)
	{
		byte = byte | ((eight[i] - '0') << i);
	}
	return byte;
}
void ShellSort(int N, tree * list[256])
{//сортировка по убыванию
	int i, j, k;
	tree *t;
	for (k = N / 2; k > 0; k /= 2)

		for (i = k; i < N; i++)
		{
			t = list[i];
			for (j = i; j >= k; j -= k)
			{
				if (t->freq > list[j - k]->freq)
					list[j] = list[j - k];
				else
					break;
			}
			list[j] = t;
		}
}
int CalculatingFrequences(FILE * from, tree * list[256], Codes * codes[256])
{
	int i, n = 0;
	unsigned char c;
	for (i = 0; i < 256; i++)
	{
		codes[i] = (Codes*)malloc(sizeof(Codes));

		list[i] = (tree*)malloc(sizeof(tree));
		list[i]->freq = 0;
		list[i]->right = list[i]->left = NULL;
	}
	while (fread(&c, 1, 1, from) == 1)
	{
		if (list[c]->freq == 0)
			n++;
		list[c]->freq++;
		list[c]->symbol = c;
	}
	rewind(from);//возвращение на начало файла (для дальнейшей работы)
	ShellSort(256, list);
	return n;
}
void Coding(FILE * before, FILE * after, Codes *codes[256])
{
	unsigned char byte = 0;
	unsigned char c;
	int i = 0, sum = 0;
	unsigned char eight[9];
	int counter = 0;
	while (fread(&c, 1, 1, before))
	{
		for (i = 0; i < strlen(codes[c]->SymbolCode); i++)
		{
			eight[sum] = codes[c]->SymbolCode[i];
			sum++;
			if (sum == 8)
			{
				sum = 0;
				eight[8] = '\0';
				byte = PackingByte(eight);
				fwrite(&byte, 1, 1, after);
				counter++;
				byte = 0;
				memset(eight, 0, 9);
			}
		}
	}
	for (i = sum; i < 8; i++)
		eight[i] = '0';
	eight[8] = '\0';
	byte = PackingByte(eight);
	fwrite(&byte, 1, 1, after);
	byte = 0;
	char end;
	end = (char)(8 - sum);//количество незадействованных битов в последнем байте
	fwrite(&end, 1, 1, after);
}
void Info(char * archive)
{
	FILE * Archive, *InfoFile;
	Archive = fopen(archive, "rb");
	unsigned long long int FileSize = 0;
	time_t Time = 0;
	unsigned char byte;
	while (fread(&byte, 1, 1, Archive))
		FileSize++;
	rewind(Archive);
	printf("\nSize of archive: %llu bytes.\n", FileSize);
	fread(&Time, sizeof(time_t), 1, Archive);
	printf("Time of archiving: %s", ctime(&Time));
	fclose(Archive);
	return;
}
int RightFileFormat(char * file)
{
	int n = strlen(file);
	if (file[n] != 'f' && file[n - 1] != 'f' && file[n - 2] != 'u' && file[n - 3] != 'h')
		return 0;
	return 1;
}