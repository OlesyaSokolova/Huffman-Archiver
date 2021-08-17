#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "OtherFunctions.h"
#include "StringsAndTrees.h"

void RecordingTree(FILE *after, unsigned char * CodingTreeString)
{
	int i = 0, sum = 0, j = 0;
	unsigned char eight[9];
	unsigned char byte = 0;
	unsigned char c;
	int lenght = strlen(CodingTreeString);
	for (i = 0; i < strlen(CodingTreeString); i++)
	{
		eight[sum] = CodingTreeString[i];
		sum++;
		if (sum == 8)
		{
			sum = 0;
			eight[8] = '\0';
			byte = PackingByte(eight);
			fwrite(&byte, 1, 1, after);
			byte = 0;
			memset(eight, 0, 9);
		}
	}
}
void TreeReconstruction1(int n, unsigned char result[65536], unsigned char * CodingTreeString)
{
	int i = 0, j = 0, k = 0;
	unsigned char eight[9];
	//n - длина строки (без дописанных нулей в конце)
	for (i = 0; i < n; i++)
	{
		result[j] = CodingTreeString[i];
		j++;
		if (CodingTreeString[i] == '1')
		{
			i++;
			for (k = 0; k < 8; k++)
			{
				eight[k] = CodingTreeString[i];
				i++;
			}
			i--;
			eight[8] = '\0';
			result[j] = ASCII_BinaryCodeToChar(eight);
			j++;
			memset(eight, 0, 9);
		}
	}
	result[j] = '\0';
}
int TreeReconstruction2(tree *new, int step, unsigned char TreeString[65536])
{
	if (TreeString[step] == '0')
	{
		tree *newLeafe1 = NULL;
		newLeafe1 = (tree*)malloc(sizeof(tree));
		newLeafe1->left = newLeafe1->right = NULL;
		new->left = newLeafe1;

		tree *newLeafe2 = NULL;
		newLeafe2 = (tree*)malloc(sizeof(tree));
		newLeafe2->left = newLeafe2->right = NULL;
		new->right = newLeafe2;

		step = TreeReconstruction2(new->left, step + 1, TreeString);
		step = TreeReconstruction2(new->right, step + 1, TreeString);
		return step;
	}
	else
	{
		step++;
		new->symbol = TreeString[step];
		new->left = NULL;
		new->right = NULL;
		return step;
	}
}