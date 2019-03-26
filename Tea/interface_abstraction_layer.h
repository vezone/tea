#pragma once

#include "bit_operations.h"
#include "tea.h"
#include "mstring.h"
#include "mio.h"

#define AllocInt32(x) (int32_t*) malloc(x*sizeof(int32_t))
#define AlloC(x) (char*)     malloc(x*sizeof(char))

//64 bit
static Data*    data;
static uint32_t BlocksCount;
static int32_t* key;

static char* FolderPath;
static char* FolderName;
static char* FolderData;
static long  FolderLength;
static int32_t LastSize;

Data* BlockSplit()
{
	Data* result;
	
	if (FolderLength > 8)
	{
		int blocksCountMinusOne = BlocksCount - 1;
		result = (Data*) malloc(BlocksCount*(sizeof(Data)));
		
		printf("BlockSplit(): BlocksCount = %d\n", BlocksCount);
		printf("BlockSplit(): blocksCountMinusOne = %d\n", blocksCountMinusOne);
		printf("BlockSplit(): FolderLength = %d\n", FolderLength);
		
		char* temp;
		for (int i = 0; i < blocksCountMinusOne; i++)
		{
			temp = mstring_get_number_of_char(FolderData, 7*i, 8);
			temp[7] = '\0';
			memcpy(result[i].string, temp, 8);
			result[i].i64 = convert_char8_to_int64(temp);
			result[i].i32[0] = get_low_bytes_from_int64(result[i].i64);
			result[i].i32[1] = get_high_bytes_from_int64(result[i].i64);
			free(temp);
			printf("BlockSplit(): data[%d] = %s\n", i, result[i].string);
		}
		
		LastSize = (FolderLength-7*(blocksCountMinusOne));
		temp = mstring_get_number_of_char(FolderData, 7*blocksCountMinusOne, LastSize);
		memcpy(result[blocksCountMinusOne].string, temp, LastSize);
		result[blocksCountMinusOne].string[LastSize] = '\0';
		result[blocksCountMinusOne].i64 = convert_char8_to_int64(temp);
		result[blocksCountMinusOne].i32[0] = get_low_bytes_from_int64(result[blocksCountMinusOne].i64);
		result[blocksCountMinusOne].i32[1] = get_high_bytes_from_int64(result[blocksCountMinusOne].i64);
		printf("BlockSplit(): data[%d] = %s\n", blocksCountMinusOne, result[blocksCountMinusOne].string);	
		printf("BlockSplit(): input = %s\nFolderLength=%d\n", FolderData, FolderLength);
		
		free(temp);
	}
	else
	{
		result = (Data*) malloc(sizeof(Data));
		result[0].i64 = convert_char8_to_int64(FolderData);
		result[0].i32[0] = get_low_bytes_from_int64(result[0].i64);
		result[0].i32[1] = get_high_bytes_from_int64(result[0].i64);
		memcpy(result[0].string, FolderData, 8);
		printf("i64 = %lld\ni32[0] = %d\ni32[1] = %d\nstring = %s\n", 
			result[0].i64, result[0].i32[0], 
			result[0].i32[1], result[0].string);
		printf("input = %s\nFolderLength=%d\n", FolderData, FolderLength);
	}
	
	return result;
}

//Высвобождение памяти
void DestroyData()
{
	if (data)
	{
		free(data);
	}
	if (key)
	{
		free(key);
	}
}

//GUI REIMPLIMENT
void Encrypt()
{
	uint32_t i;
	for (i = 0; i < BlocksCount; i++)
	{
		encrypt_data(data, key, i);
		printf("Encrypted string[%d] = %s\n", i, data[i].string);
	} 
}


//Упрощение для обработчика нажатия на Decrypt
void Decrypt()
{
	uint32_t i;
	for (i = 0; i < BlocksCount; i++)
	{
		decrypt_data(data, key, i);
		printf("Decrypted string[%d] = %s\n", i, data[i].string); 
	}
}

//Упрощение для обработчика нажатия на SaveFile
void SaveFile()
{
	const char* savePath = mstring_get_string_to(FolderPath, '.');
	if (FolderLength > 8)
	{
		char* result = (char*) malloc(FolderLength*sizeof(char));
		uint32_t i;
		for (i = 1; i < BlocksCount; i++)
		{
			printf("data[%d] = %s\n", i, data[i].string);
			result = mstring_concat(result, data[i].string);
		}
		savePath = mstring_concat(savePath, ".enc");
		mio_write_binary_file(savePath, "wb+", result, FolderLength);
		free(result);
	}
	else 
	{
		char* result = (char*) malloc(8*sizeof(char));
		memcpy(result, data[0].string, 8);
		mio_write_binary_file(savePath, "wb+", result, 8);
		free(result);
	}
}

void folder_changed_laba1_event(char* path)
{
	key =  AllocInt32(4);
	key[0] = 0;
	key[0] = 1;
	key[0] = 2;
	key[0] = 3;
	FolderPath = path;
	FolderName = mstring_get_string_to(FolderPath, '.');
	//проверять есть ли в path 
	//if !contains(".txt") бинарный 
	//else текстовый
	FolderData = 
			mio_read_text_file(FolderPath, "r", &FolderLength); 
	if (FolderLength > 8)
	{
		//mio_read_binary_file(FolderPath, "r", &FolderLength);
		printf("FolderChanged(): FolderData = %s\n", FolderData);
		BlocksCount = (int)(FolderLength/7.0f + 1.0f);
	}
	else 
	{
		FolderData = 
			mio_read_binary_file(FolderPath, "r", &FolderLength); 
		BlocksCount = 1;
	}
	
	data = BlockSplit();
}

//Упрощение для обработчика нажатия на FolderChanged
void folder_changed_laba3_event(char* path)
{
	FolderPath = path;
	FolderName = mstring_get_string_to(FolderPath, '.');
	
	key =  AllocInt32(4);
	int length;
	char* key_string = mio_read_text_file("/home/bies/Dev/Programming/C/InformationSecurity/Laba2/key.enc", "rb", &length);
	//WriteFile("new_file_name_file_name.txt", "info", 4);
	if (strcmp(key_string, "error") != 0)
	{
		if (key_string)
		{
			char* temp = strtok(key_string, "\n");
			key[0] = atoi(temp);
			for (int i = 1 ; ; i++) 
			{
				temp = strtok(NULL, "\n");
				if (temp == NULL) break;
				key[i] = atoi(temp);
			}
		}
	}
	//проверять есть ли в path 
	//if !contains(".txt") бинарный 
	//else текстовый
	FolderData = 
			mio_read_text_file(FolderPath, "r", &FolderLength); 
	if (FolderLength > 8)
	{
		//mio_read_binary_file(FolderPath, "r", &FolderLength);
		printf("FolderChanged(): FolderData = %s\n", FolderData);
		BlocksCount = (int)(FolderLength/7.0f + 1.0f);
	}
	else 
	{
		FolderData = 
			mio_read_binary_file(FolderPath, "r", &FolderLength); 
		BlocksCount = 1;
	}
	
	data = BlockSplit();
	
}

