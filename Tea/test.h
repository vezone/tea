
#define BLOCK_SIZE 7.0f

#include "mstring.h"
#include "mio.h"

//тестовая функция
void additional_test()
{
	
	/*
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
	*/
	
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

//тестовая функция
void func()
{
	int len;
	char* str = mio_read_binary_file("/home/bies/Dev/Programming/C/InformationSecurity/Laba1/file.txt", &len);
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
		parStr[i] = mstring_get_number_of_char(str, BLOCK_SIZE*i, BLOCK_SIZE+1);
		memcpy((void*)pdata[i].string, parStr[i], BLOCK_SIZE+1);
		g_print("parStr[%d] = %s\n", i, pdata[i].string);
	}
	int lastSize = (len-BLOCK_SIZE*nblocksl);
	g_print("lastSize = %d\n", lastSize);
	parStr[nblocksl] = mstring_get_number_of_char(str, BLOCK_SIZE*nblocksl, lastSize);
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