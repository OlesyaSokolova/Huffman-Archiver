#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h> 
#include "OtherFunctions.h"
#include "Tree.h"
#include "StringsAndTrees.h"

void Dearchiving(char *before, char * after)
{
	
	FILE * Archive, *NewFile;
	if (!fopen(before, "rb"))
	{
		printf("Unfortunately, file for decompressing can't be opened! Check if it exists and locates in the folders 'Debug' and 'arc2'.");
		getchar();
		return;
	}
	if (!fopen(after, "rb"))
	{
		printf("Unfortunately, file in which you are going to decompress previous file can't be opened! Check if it exists and locates in the folder 'Debug' and 'arc2'.");
		getchar();
		return;
	}
	Archive = fopen(before, "rb");
	NewFile = fopen(after, "wb");
	printf("Decompressing...Please, wait...\n");
	int length;
	unsigned long long int counter = 0;
	unsigned char byte;
	char rest;
	time_t Time;
	while (fread(&byte, 1, 1, Archive))
		counter++;
	if (counter == 0)
	{
		printf("File exists but has no information (nothing to dearchive).");
		getchar();
		return;
	}
	rest = byte;
	rewind(Archive);
	fread(&Time, sizeof(time_t), 1, Archive);
	fread(&length, 4, 1, Archive);
	
	counter = counter - length - 1 - sizeof(time_t);
	int b, i, j, k;
	unsigned char C[65536];
	k = 0;
	for (i = 0; i < length; i++)
	{
		fread(&byte, 1, 1, Archive);
		for (j = 0; j < 8; j++)
		{
			b = (byte & 1);
			byte >>= 1;
			C[k] = (unsigned char)(b + '0');
			k++;
		}
	}
	C[k] = '\0';

	unsigned char TreeString[65536];
	TreeReconstruction1(length*8, TreeString, C);
	tree * new = NULL, *head;
	new = (tree*)malloc(sizeof(tree));
	head = new;
	int step = 0;
	step = TreeReconstruction2(new, step, TreeString);
	//head = CodingTree;
	head = new;
	//rest - количество незадействованных битов в последнем байте
	tree * root = head;
	char end = 8;
	if (root->right == NULL || root->left == NULL)
	{
		fwrite(&root->symbol, 1, 1, NewFile);
		fclose(Archive);
		fclose(NewFile);
		return;
	}
		
	for(k = 0; k < counter - 5; k++)
	{
		fread(&byte, 1, 1, Archive);
		for (i = 0; i < end; i++)
		{
			if ((byte & (1 << i)) == 0)
			{
				root = root->left;
				if (root->left == NULL && root->left == NULL)
				{
					fwrite(&root->symbol, 1, 1, NewFile);
					root = head;
				}

			}
			else
			{
				root = root->right;
				if (root->left == NULL && root->left == NULL)
				{
					fwrite(&root->symbol, 1, 1, NewFile);
					root = head;
				}
			}
		}
	}
	fread(&byte, 1, 1, Archive);
	for (i = 0; i < 8 - rest; i++)
	{
		if ((byte & (1 << i)) == 0)
		{
			root = root->left;
			if (root->left == NULL && root->left == NULL)
			{
				fwrite(&root->symbol, 1, 1, NewFile);
				root = head;
			}

		}
		else
		{
			root = root->right;
			if (root->left == NULL && root->left == NULL)
			{
				fwrite(&root->symbol, 1, 1, NewFile);
				root = head;
			}
		}
	}
	printf("\nDecompressed successfully!\n");
	fclose(Archive);
	fclose(NewFile);
}
void Archiving(char *before, char * after)
{
	FILE *FileToArc, *Archive;
	if (!fopen(before, "rb"))
	{
		printf("Unfortunately, file for compressing can't be opened! Check if it exists and locates in the folder 'Debug' and 'arc2'.");
		return;
	}
	if (!fopen(after, "rb"))
	{
		printf("Unfortunately, file in which you are going to compress previous file can't be opened! Check if it exists and locates in the folder 'Debug' and 'arc2'.");
		return;
	}
	FileToArc = fopen(before, "rb");
	Archive = fopen(after, "wb");
	printf("Compressing...Please, wait...\n");
	tree *list[256];
	Codes *codes[256];
	int n, i;
	int length = 0;
	char rest = 0;
	unsigned char *CodingTreeString;
	unsigned char c;
	time_t Time = time(NULL);
	fwrite(&Time, sizeof(time_t), 1, Archive);
	tree *CodingTree;
	n = CalculatingFrequences(FileToArc, list, codes);
	if (n == 0)
	{
		printf("File exists but has no information (nothing to archive).");
		getchar;
		return;
	}	
	CodingTreeString = (unsigned char*)calloc(n * 256, sizeof(unsigned char));
	CodingTree = CreatingCodingTree(n, list);
	CreatingCodingTreeString(CodingTreeString, CodingTree);

	if (n != 1)
	{
		strcpy(CodingTree->left->code, "0");
		strcpy(CodingTree->right->code, "1");
		CreatingCodes(CodingTree->left, codes);
		CreatingCodes(CodingTree->right, codes);
	}
	else
		strcpy(codes[CodingTree->symbol]->SymbolCode, "0");
	length = strlen(CodingTreeString);
	rest = (char)length % 8;
	for (i = length; i < length + (8 - rest); i++)
		CodingTreeString[i] = '0';
	//////////////////////////////////////////////////////////////////////////////
	//структура архива: время архивации | длина дерева (в байтах)  | само дерево | архив| информация о конце файла
	length = length + (8 - rest);
	length = (int)length / 8;
	fwrite(&length, 4, 1, Archive);
	RecordingTree(Archive, CodingTreeString);
	Coding(FileToArc, Archive, codes);
	printf("\nCompressed successfully!\n");
	fclose(Archive);
	fclose(FileToArc);
	
}

int main(int argc, char * argv[])
{	
	if (argv[1] == NULL)
	{
		printf("Hello! Are you going to use 'arc2.exe'?\nIf so, please enter the name of files that you \nwant to compress or decompress.If you need more information or help,\nrun the program again, enter 'h' and press 'Enter'. Good luck!");
		getchar();
		return 0;
	}
	if (strcmp(argv[1], "h") == 0)
	{
		printf("INFORMATION ABOUT THE PROGRAM 'arc2.exe':\n");
		printf("This program can compress and decompress\n files. (It is based on Huffman algorithm.)\nHere you can read how to use it:\n");
		printf("0. First of all, add files that you want to compress\n or decompress in the folder 'Debug' and 'arc2'.\n(File which you want to compress in or decompress from\n should also be in this folder and have format '.huff')\n");
		printf("1. If you want to compress file, enter the file name\n and name of a file-archive. For example,\n 'arc2.exe c book.txt arc.huff'.\n");
		printf("2. If you want to decompress file, enter the file-archive\n name and a name of file-result. For example,\n 'arc2.exe d arc.huff book2.txt'.\n");
		printf("3. If you want to see information about file-archive, enter 'i' before file-archive name and press 'Enter'.\n");
		printf("4. If you want to compress and then decompress file, enter \nthe file name, a file-archive name, and a file-result name. For example, \n 'c d arc2.exe book.txt arc.huff book2.txt'. Be carefull with the order!\n ");
		
		printf("What is more,\n-You cannot compress several files at the same time.\n");
		printf("-You cannot open and read file-archive.\n");
		printf("-If you do something wrong, the program will say what the problem is and what to do.\n");
		printf("-Remember that formats of files that you compress and the decompress are the same\n");
		printf("-If file is too little, its file-archive can be even bigger.\n");
		printf("-If file is quite big, it can took time to compress or decompress it.\n");

		printf("-Operations that arc2.exe can do with your files:\n");
		printf("     'c' - compress file (example: 'arc2.exe c book.txt arc.huff');\n");
		printf("     'i' - information about file (only for file-archives, example: 'arc2.exe i arc.huff');\n");
		printf("     'd' - decompress file (example: 'arc2.exe d arc.huff book.txt')\n");

		printf("-Good luck!(Press 'Enter' to close the instruction.)");
		getchar();
		return 0;
	}
	if (strcmp(argv[1], "c") == 0 && argv[2] != NULL && argv[3]!= NULL && RightFileFormat(argv[3]))
		Archiving(argv[2], argv[3]);
	if (strcmp(argv[1], "d") == 0 && argv[2] != NULL && argv[3] != NULL && RightFileFormat(argv[2]))
		Dearchiving(argv[2], argv[3]);

	if (strcmp(argv[1], "c") == 0 && argv[2] != NULL && argv[3] != NULL && !RightFileFormat(argv[3]))
	{
		printf("Wrong format of second file (it should be'.huff')! Please, try again.\nIf you need help, run program againg and enter 'h'.");
		getchar();
		return 0;
	}
	if (strcmp(argv[1], "d") == 0 && argv[2] != NULL && argv[3] != NULL && !RightFileFormat(argv[2]))
	{
		printf("Wrong format of first file (it should be'.huff')! Please, try again.\nIf you need help, run program againg and enter 'h'.");
		getchar();
		return 0;
	}

	if (strcmp(argv[1], "d") == 0 && argv[2] != NULL && argv[3] != NULL && !RightFileFormat(argv[2]))
		Dearchiving(argv[2], argv[3]);

	if (strcmp(argv[1], "c") == 0 && strcmp(argv[2], "d") == 0 && argv[3] != NULL && argv[4] != NULL && argv[5] != NULL)
	{
		Archiving(argv[3], argv[4]);
		Dearchiving(argv[4], argv[5]);
	}
	if (strcmp(argv[1], "c") == 0 && strcmp(argv[2], "d") == 0 && argv[3] != NULL && argv[4] != NULL && argv[5] == NULL)
	{
		printf("You forgot to enter last file in which you \nwant to decompress your file! Please, try again.\n If you need help, run program againg and enter 'h'.");
		getchar();
		return 0;
	}
	if (strcmp(argv[1], "c") == 0 && strcmp(argv[2], "d") == 0 && argv[3] != NULL && argv[4] == NULL)
	{
		printf("You forgot to enter file in which you \nwant to compress first entered file! Please, try again.\n If you need help, run program againg and enter 'h'.");
		getchar();
		return 0;
	}
	if (strcmp(argv[1], "c") == 0 && strcmp(argv[2], "d") == 0 && argv[3] == NULL)
	{
		printf("You forgot to enter files! Please, try again.\nIf you need help, run program againg and enter 'h'.");
		getchar();
		return 0;
	}
	if (strcmp(argv[1], "c") == 0 && strcmp(argv[2], "d") == 0 && argv[3] == NULL)
	{
		printf("You forgot to enter file-archive and file-result! Please, try again.\nIf you need help, run program againg and enter 'h'.");
		getchar();
		return 0;
	}
	if (strcmp(argv[1], "c") == 0 && argv[2] != NULL && argv[3] == NULL)
	{
		printf("You forgot to enter file-archive! Please, try again.\nIf you need help, run program againg and enter 'h'.");
		getchar();
		return 0;
	}
	if (strcmp(argv[1], "d") == 0 && argv[2] != NULL && argv[3] == NULL)
	{
		printf("You forgot to enter file-result! Please, try again.\nIf you need help, run program againg and enter 'h'.");
		getchar();
		return 0;
	}
	if (strcmp(argv[1], "c") != 0 && strcmp(argv[1], "d") != 0 && strcmp(argv[1], "i") != 0 && strcmp(argv[1], "h") != 0)
	{
		printf("You forgot to write what to do or operation is unknown! The program can only:\n");
		printf("     'c' - compress file (example: 'arc2.exe c book.txt arc.huff');\n");
		printf("     'i' - information about file (only for file-archives, example: 'arc2.exe i arc.huff');\n");
		printf("     'd' - decompress file (example: 'arc2.exe d arc.huff book.txt')\n");
		printf("Or, maybe, you entered a name of a file without any operations.\n");
		printf("Please, try again.\nIf you need help, run program againg and enter 'h'.");
		getchar();
		return 0;
	}
	if (strcmp(argv[1], "i") == 0 && RightFileFormat(argv[2]) == 0)
	{
		printf("Wrong format of file (it should be'.huff')! Please, try again.\nIf you need help, run program againg and enter 'h'.");
		getchar();
		return 0;
	}
		
	return 0;
}