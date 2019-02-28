#pragma once

#define BEG 0
#define CUR 1
#define END 2

//чтение файла
char* ReadFile(char fileName[255], long* length)
{
	char* readedString = 0;
	FILE* file;
	file = fopen(fileName, "r");
	if (file != NULL)
	{
		fseek(file, 0, END);
		*length = ftell(file);
		fseek(file, 0, BEG);
		readedString = (char*)malloc(*length*sizeof(char));
		fread((void*)readedString, *length*sizeof(char), *length, file);
		fclose(file);
	}
	else 
	{
		//file read error
		g_print("File read error\n");
	}
	
	return readedString;
}

//Безопасное чтение файла
char* ReadFileSafe(char fileName[255], int* isFileExist, long* length)
{
	FILE* file;
	file = fopen(fileName, "r");
	char* output;
	
	if (file != NULL)
	{
		fseek(file, 0, END);
		*length = ftell(file);
		fseek(file, 0, BEG);
		output = (char*)malloc(*length*sizeof(char));
		fread((void*)output, *length*sizeof(char), *length, file);
		fclose(file);
		*isFileExist = 1;
		return output;
	}
	else 
	{
		g_print("File read error\n");
		*isFileExist = -1;
		return "error";
	}
}

//Запись в файл
void WriteFile(char fileName[255], char* data, int length)
{
	FILE* file;
	file = fopen(fileName, "wb+");
	if (file != NULL)
	{	
		fwrite(data, 1, length, file);
		fclose(file);
	}
	else 
	{
		//file read error
		g_print("File write error\n");
	}
}

//Запись с дополнением файла
void WriteAppendFile(char fileName[255], char* data, int length)
{
	FILE* file;
	file = fopen(fileName, "a+");
	if (file != NULL)
	{	
		if (data != NULL && length > 0)
		{
			fwrite(data, 1, length, file);
		}
		fclose(file);
	}
	else 
	{
		//file read error
		g_print("File write error\n");
	}
}