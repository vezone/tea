#pragma once

#define BEG 0
#define CUR 1
#define END 2

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