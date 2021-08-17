#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "OtherFunctions.h"
#include "Tree.h"

tree * CreatingCodingTree(int n, tree * list[256])
{
	int i, j;
	tree *NewNode = NULL;
	if (n == 1)
	{
		NewNode = (tree*)malloc(sizeof(tree));
		NewNode->freq = list[0]->freq;
		NewNode->right = NULL;
		NewNode->left = NULL;
		NewNode->symbol = list[0]->symbol;
	}
	while (n != 1)
	{
		NewNode = (tree*)malloc(sizeof(tree));
		NewNode->freq = list[n - 1]->freq + list[n - 2]->freq;
		NewNode->right = list[n - 1];
		NewNode->left = list[n - 2];
		int i, j;
		for (i = 0; i < n; i++)
		{//вставка нового узла в массив
			if (NewNode->freq > list[i]->freq)
			{
				for (j = n - 1; j > i; j--)
					list[j] = list[j - 1];//сдвиг всех элементов слева от места вставки
				list[i] = NewNode;//вставка
				break;
			}
		}
		n--;
	}
	return NewNode;
}
void CreatingCodes(tree *CodingTree, Codes * codes[256])
{
	if (CodingTree->left != NULL)
	{

		strcpy(CodingTree->left->code, CodingTree->code);
		strcat(CodingTree->left->code, "0");
		CreatingCodes(CodingTree->left, codes);
	}
	if (CodingTree->right != NULL)
	{
		strcpy(CodingTree->right->code, CodingTree->code);
		strcat(CodingTree->right->code, "1");
		CreatingCodes(CodingTree->right, codes);
	}
	else
	{
		strcpy(codes[CodingTree->symbol]->SymbolCode, CodingTree->code);
		codes[CodingTree->symbol]->c = CodingTree->symbol;
	}
}
void CreatingCodingTreeString(unsigned char *CodingTreeString, tree * CodingTree)
{
	int length;
	if (CodingTree->left != NULL)
	{
		length = strlen(CodingTreeString);
		CodingTreeString[length] = '0';
		CodingTreeString[length + 1] = '\0';
		CreatingCodingTreeString(CodingTreeString, CodingTree->left);
	}
	if (CodingTree->right != NULL)
	{
		//length = strlen(CodingTreeString);
		//CodingTreeString[length] = '0';
		//CodingTreeString[length + 1] = '\0';
		CreatingCodingTreeString(CodingTreeString, CodingTree->right);
	}
	else
	{
		length = strlen(CodingTreeString);
		CodingTreeString[length] = '1';
		/*CodingTreeString[length + 1] = CodingTree->symbol;
		CodingTreeString[length + 2] = '\0';*/
		CodingTreeString[length + 1] = '\0';
		strcat(CodingTreeString, BinarySymbolString(CodingTree->symbol));
		length = strlen(CodingTreeString);
		CodingTreeString[length + 1] = '\0';
	}
}