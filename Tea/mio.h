#pragma once

#define BEG 0
#define CUR 1
#define END 2

//чтение файла
char* mio_read_binary_file(char fileName[255], char* mode, long* length)
{
	char* readedString = 0;
	FILE* file;
	file = fopen(fileName, mode);
	if (file != NULL)
	{
		fseek(file, 0, END);
		*length = ftell(file);
		fseek(file, 0, BEG);
		readedString = (char*)malloc((*length)*sizeof(char));
		fread(readedString, sizeof(char), (*length), file);
		fclose(file);
	}
	else 
	{
		//file read error
		g_print("File read error\n");
	}
	
	return readedString;
}

//Запись в файл 
// mode: "wb+" - write_binary_file; "a+" - append_file 
void mio_write_binary_file(char fileName[255], const char* mode, char* data, int length)
{
	FILE* file;
	file = fopen(fileName, mode);
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
		//file write error
		g_print("File write error\n");
	}
}

void mio_write_text_file(char fileName[255], char* data)
{
	FILE* file;
	file = fopen(fileName, "a+");
	if (file != NULL)
	{
		if (data != NULL)
		{
			fputs(data, file);
		}
		fclose(file);
	}
	else 
	{
		//file write error
		g_print("File write error\n");
	}
}

char* mio_read_text_file(char fileName[255], const char* mode, long* length)
{
	FILE* file;
	file = fopen(fileName, mode);
	if (file != NULL)
	{
		fseek(file, 0, END);
		*length = ftell(file);
		fseek(file, 0, BEG);
		int iter = 0;
		char* readed_string = (char*) malloc((*length)*sizeof(char));
		char* readed_string_final = (char*) malloc((*length)*sizeof(char));
		char* eof_string;
		while (1)
		{
			//strncat
			eof_string = fgets(readed_string, (*length), file);
			if (eof_string == NULL)
			{
				if (feof(file) != 0)
				{
					printf("Finished reading file!\n");
					break;
				}
				else
				{
					printf("file_read_error!\n");
					break;
				}
			}
			if (iter > 0)
			{
				readed_string_final = mstring_concat(readed_string_final, (const char*)readed_string);
			}
			else
			{
				readed_string_final = mstring_concat("", (const char*)readed_string);
			}
			++iter;
		}
		
		//printf("readed_string_final = %s\n", readed_string_final);
		fclose(file);
		return readed_string_final;
	}
	else
	{
		return "error";
	}
}

char* mio_read_line_text_file(char fileName[255], const char* mode, long* length)
{
	FILE* file;
	file = fopen(fileName, mode);
	if (file != NULL)
	{
		fseek(file, 0, END);
		*length = ftell(file);
		fseek(file, 0, BEG);
		char* readed_string = (char*) malloc((*length)*sizeof(char));
		fgets(readed_string, (*length), file);
		fclose(file);
		return readed_string;
	}
	else
	{
		return "error";
	}
}