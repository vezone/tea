#pragma once

typedef struct {
	int Length;
	int CurrentElement;
	char** Buffer;
} StringArray;

StringArray string_array_allocate(int length)
{
	StringArray result;
	result.Length = length;
	result.CurrentElement = 0;
	result.Buffer = (char**) malloc(length*sizeof(char));
	return result;
}

void string_array_add_character(StringArray* this_array, char character_to_add)
{
	this_array->Buffer[this_array->CurrentElement] = (char*) malloc (2*sizeof(char));
	this_array->Buffer[this_array->CurrentElement][0] = character_to_add;
	this_array->Buffer[this_array->CurrentElement][1] = '\0';
	this_array->CurrentElement++;
}

void string_array_add_string(StringArray* this_array, char* string_to_add)
{
	int string_to_add_length = strlen(string_to_add);
	this_array->Buffer[this_array->CurrentElement] = (char*) malloc((string_to_add_length+1)*sizeof(char));
	for (int i = 0; i < string_to_add_length; i++)
	{
		this_array->Buffer[this_array->CurrentElement][i] = string_to_add[i];
	}
	this_array->Buffer[this_array->CurrentElement][string_to_add_length] = '\0';
	this_array->CurrentElement++;
}

int string_array_contains_character(StringArray* this_array, char character_to_check)
{
	int length = (this_array->CurrentElement);
	int i;
	for (i = 0; i < length; i++)
	{
		if (this_array->Buffer[i][0] == character_to_check)
		{
			return(1);
		}
	}
	return(0);
}

int string_array_contains_string(StringArray* this_array, char* string_to_check)
{
	int length = (this_array->CurrentElement);
	int i;
	for (i = 0; i < length; i++)
	{
		if (strcmp(this_array->Buffer[i], string_to_check) == 0)
		{
			return(1);
		}
	}
	return(0);
}

int string_array_get_index_of_string(StringArray* this_array, char* string_to_check)
{
	int length = (this_array->CurrentElement);
	int i;
	for (i = 0; i < length; i++)
	{
		if (strcmp(this_array->Buffer[i], string_to_check) == 0)
		{
			return(i);
		}
	}
	return(-1);
}

void string_array_write_to_file(char fileName[255], StringArray* this_array)
{
	int length = (this_array->CurrentElement-1);
	
	for (int i = 0; i < length; i++)
	{
		mio_write_text_file(fileName, mstring_concat(this_array->Buffer[i], "\n"));
	}
	
	mio_write_text_file(fileName, this_array->Buffer[length]);
}

void print_string_array(StringArray* this_array)
{
	int length = (this_array->CurrentElement);
	for (int i = 0; i < length; i++)
	{
		printf("this_array[%d] = %s\n", i, this_array->Buffer[i]);
	}
}

typedef struct {
	StringArray Dictionary;
	char* Input;
} LZW;

void lzw_init(LZW* lzw)
{
	lzw->Input = "TOBEORNOTTOBEORTOBEORNOT";
	
	lzw->Dictionary = string_array_allocate(4096);
	
	char* string;
	int input_string_length = strlen(lzw->Input);
	int temp_length = (input_string_length-1);
	for (int i = 0; i < temp_length; i+=2)
	{
		string = mstring_concat_chars(lzw->Input[i], lzw->Input[i+1]);
		if (!string_array_contains_string(&lzw->Dictionary, string))
		{
			string_array_add_string(&lzw->Dictionary, string);
		}
	}
	
	if ((input_string_length % 2) != 0)
	{
		string_array_add_character(&lzw->Dictionary, lzw->Input[temp_length]);
	}
	
	print_string_array(&lzw->Dictionary);
}

char* lzw_compression(LZW* lzw)
{
	char* result = (char*)malloc(1);
	result[0] = '\0';
	char* string;
	int i = 0, index = 0;
	int lzw_input_length = strlen(lzw->Input);
	for (i = 0; i < lzw_input_length; i+=2)
	{
		string = mstring_concat_chars(lzw->Input[i], lzw->Input[i+1]);
		index = string_array_get_index_of_string(&lzw->Dictionary, string);
		if (index != -1)
		{
			char* temp = mstring_convert_int32_to_string(index);
			result = mstring_concat(result, temp);
		}
	}
	
	return result;
}

char* lzw_decompression(LZW* lzw, char* input_string)
{
	char* result = (char*)malloc(1);
	result[0] = '\0';
	int index_of_string;
	int i = 0;
	int lzw_dictionary_length = strlen(input_string);
	for (i = 0; i < lzw_dictionary_length; i++)
	{
		index_of_string = (input_string[i]-48);
		char* temp = lzw->Dictionary.Buffer[index_of_string];
		result = mstring_concat(result, temp);
	}
	
	return result;
}

void lzw_test()
{
	//dictionary size 4096
	
	LZW lzw;
	lzw_init(&lzw);
	char* result = lzw_compression(&lzw);
	printf("result = %s\n", result);
	char* decomp = lzw_decompression(&lzw, result);
	printf("decomp = %s\n", decomp);
	
	if (strcmp(lzw.Input, decomp) == 0)
	{
		printf("CORRECT!\n");
	}
	else
	{
		printf("WRONG!\n");
	}
	
	//r/d = 0.5 2*d/(r) = 2
	float compression_rank = (float)strlen(decomp) / (float)strlen(result);
	
	printf("result = %s\ndecomp = %s\nrank = %f\n", 
		result, decomp, compression_rank);
	
	
}