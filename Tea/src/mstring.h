#pragma once

//Взятие подстроки ОТ определенного индекса до конца
char* GetCharFrom(char* string, int from)
{
	if (string != NULL)
	{
		int newLength = strlen(string)-from;
		int i, index;
		char* newString = (char*)malloc(newLength*sizeof(char));
		for (i = from, index = 0; i < newLength; i++)
		{
			newString[index] = string[i];
			++index; 
		}
		return newString;
	}
	else
	{
		return "error";
	}
}

//Взяятие подстроки до определенного сивола
char* GetCharTo(char* string, char character)
{
	int i, index = 0;
	int length = strlen(string);
	for (i = 0; i < length; i++)
	{
		if (string[i] == '.')
		{
			index = i;
			break;
		}
	}
	
	if (index == 0)
		index = length;
	
	i = 0;	
	char* newString = (char*)malloc(index*sizeof(char));
	for (i = 0; i < index; i++)
	{
		newString[i] = string[i];
	}
	
	return newString;
}

//Взятие N byte от строки
char* GetNByte(char* string, int from, int nByte)
{
	if (nByte > 0)
	{
		int index, nindex = 0,
			indexTo = from + nByte - 1;
		char* newString = (char*)malloc((nByte) * sizeof(char));
		for (index = from; index < indexTo; index++)
		{
			newString[nindex] = string[index];
			nindex++;
		}
		newString[indexTo] = '\0';
		return newString;
	}
	else
	{
		return "error";
	}
} 

//Set строки нулевыми символами
void SetNull(char* string, int length)
{
	for (int i = 0; i < length; i++)
	{
		string[i] = '\0';
	}
}
	
#define BLOCK_SIZE 7.0f
#include "mio.h"

//тестовая функция
void func()
{
	int len;
	char* str = ReadFile("/home/bies/Dev/Programming/C/InformationSecurity/Laba1/file.txt", &len);
	g_print("str = %slen = %i\n", str, len);
	int nblocks = (int)(len / BLOCK_SIZE+1.0f);
	g_print("nblocks = %d\n", nblocks);
	
	//-------------------sizeof(Data)*numberOfBlocks
	char** parStr = (char**) malloc(8*nblocks);
	for (int i = 0; i < nblocks; i++)
	{
		parStr[i] = (char*) malloc(8);
	}
	
	int nblocksl = nblocks - 1;
	Data* pdata = (Data*) malloc(nblocks*(sizeof(Data)));
	for (int i = 0; i < nblocksl; i++)
	{
		parStr[i] = GetNByte(str, BLOCK_SIZE*i, BLOCK_SIZE+1);
		memcpy((void*)pdata[i].string, parStr[i], BLOCK_SIZE+1);
		g_print("parStr[%d] = %s\n", i, pdata[i].string);
	}
	int lastSize = (len-BLOCK_SIZE*nblocksl);
	g_print("lastSize = %d\n", lastSize);
	parStr[nblocksl] = GetNByte(str, BLOCK_SIZE*nblocksl, lastSize);
	memcpy((void*)pdata[nblocksl].string, parStr[nblocksl], lastSize);
	g_print("parStr[%d] = %s\n", nblocksl, parStr[nblocksl]);	
	
	//g_print("for cicle\n");
	for(int i = 0; i < nblocksl; i++)
	{
		WriteAppendFile("file.txt", 
			parStr[i], 7);
	}
	WriteAppendFile("file.txt", parStr[nblocksl], lastSize-1);
	
	//concat string
	/*char* concat = (char*) malloc(len*sizeof(char));
	for (int i = 0; i < nblocks; i++)
	{
		//char* temp = parStr[i];
		g_print("temp=%s\n", parStr[i]);
		for (int j = 0; j < 7; j++)
		{ 
			char temp = parStr[i][j];
			concat[i*8 + j] = temp;
		}
	}
	
	for (int i = 0; i < len; i++)
	{
		g_print("c[%d] = %c\n", i, concat[i]);
	}*/
	
	//g_print("concat = %s\n", concat);
}
