#pragma once

#include "tea.h"
#include "mstring.h"
#include "mio.h"

#define Alloc(x) (uint32_t*) malloc(x*sizeof(uint32_t))
#define AlloC(x) (char*) malloc(x*sizeof(char))

//64 bit
static Data* data;
static unsigned int BlocksSize;
static unsigned int BlocksCount;
static uint32_t* key;

static char* FolderPath;
static char* FolderName;
static char* FolderData;
static long  FolderLength;

static int LastSize;

Data* BlockSplit()
{
	BlocksSize = 7;
	//g_print("input = %slen = %i\n", FolderData, FolderLength);
	Data* result;
	
	if (FolderLength > 8)
	{
		BlocksCount = (int)(FolderLength / BlocksSize+1.0f);
		result = (Data*) malloc(BlocksCount*(sizeof(Data)));
		int nblocksl = BlocksCount - 1;
		//g_print("nblocks = %d\n", BlocksCount);
		char* temp = (char*) malloc(8*sizeof(char));
		for (int i = 0; i < nblocksl; i++)
		{
			temp = GetNByte(FolderData, BlocksSize*i, BlocksSize+1);
			memcpy((void*)result[i].string, temp, BlocksSize+1);
			//g_print("pdata[%d] = %s\n", i, result[i].string);
		}
		
		LastSize = (FolderLength-BlocksSize*nblocksl);
		temp = GetNByte(FolderData, BlocksSize*nblocksl, LastSize);
		//g_print("temp = %s\n", temp);	
		memcpy((void*)result[nblocksl].string, temp, LastSize);
		//g_print("pdata[%d] = %s\n", nblocksl, result[nblocksl].string);	
		//g_print("input = %s\nFolderLength=%d\n", FolderData, FolderLength);
	}
	else
	{
		result = (Data*) malloc(sizeof(Data));
		BlocksCount = 1;
		memcpy((void*)result[0].string, FolderData, FolderLength-1);
	}
	
	return result;	
}

void WriteBlockFile(char fileName[255], unsigned int lastSize)
{
	int i;
	unsigned int nblocksl = BlocksCount - 1;
	
	for(i = 0; i < nblocksl; i++)
	{
		WriteAppendFile(fileName, 
			data[i].string, BlocksSize);
	}
	WriteAppendFile(fileName, data[nblocksl].string, lastSize-1);
}

void DataInit()
{
	key =  Alloc(4);
	key[0] = 1;
	key[1] = 2;
	key[2] = 3;
	key[3] = 4;
	
	//int length;
	//char* keyStr = ReadFile("key.txt", &length);
	//strtok(keyStr, "\n");
}

void DestroyData()
{
	if (key)
	{
		free(key);
	}
}

//GUI REIMPLIMENT

void Encrypt()
{
	for (int i = 0; i < BlocksCount; i++)
	{
		EncryptData(&data[i].string, key);
		g_print("Encrypted string[%d] = %s\n", i, data[i].string); 
	}
}

void Decrypt()
{
	for (int i = 0; i < BlocksCount; i++)
	{
		DecryptData(&data[i].string, key);
		g_print("Decrypted string[%d] = %s\n", i, data[i].string); 
	}
}

void SaveFile()
{
	char* savePath = GetCharTo(FolderPath, '.');
	WriteBlockFile(savePath, LastSize);
}

void FolderChanged(char* path)
{
	FolderPath = path;
	FolderName = GetCharTo(FolderPath, '.');
	
	FolderData = ReadFile(FolderPath, &FolderLength);
	LastSize = (FolderLength-BlocksSize*(BlocksCount-1));
	data = BlockSplit(FolderData);
}

void entry()
{
	FolderPath = "/home/bies/Dev/Programming/C/InformationSecurity/Laba1/file.txt";
	FolderData = ReadFile(FolderPath, &FolderLength);
	FolderName = GetCharTo(FolderPath, '.');
	BlocksSize = 7;
	BlocksCount = (int)(FolderLength / BlocksSize+1.0f);
	int nblocksl = BlocksCount-1;
	LastSize = (FolderLength-BlocksSize*nblocksl);
	data = BlockSplit();
	
	key =  Alloc(4);
	key[0] = 1;
	key[1] = 2;
	key[2] = 3;
	key[3] = 4;
	
	for (int i = 0; i < BlocksCount; i++)
	{
		Encrypt(data[i].string, key);
	}
	
	WriteBlockFile(FolderName, LastSize);
	
	//FolderData = ReadFile(FolderName, &FolderLength);
	//BlocksSize = 14;
	//BlocksCount = (int)(FolderLength / BlocksSize+1.0f);
	//LastSize = (FolderLength-BlocksSize*nblocksl);
	//data = BlockSplit();
	//
	//for (int i = 0; i < BlocksCount; i++)
	//{
	//	Decrypt(data[i].string, key);
	//}
	
}