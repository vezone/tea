#pragma once

//Взятие подстроки ОТ определенного индекса до конца
char* mstring_get_string_from(char* string, int from)
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
char* mstring_get_string_to(char* string, char character)
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
char* mstring_get_number_of_char(char* input_string, int from, int number_of_byte_to_copy)
{
	if (number_of_byte_to_copy > 0)
	{
		int old_index, new_index = 0,
			index_to = from + number_of_byte_to_copy;
		char* new_string = (char*)malloc((number_of_byte_to_copy+1) * sizeof(char));
		for (old_index = from; 
			 old_index < index_to; 
			 old_index++,
			 new_index++)
		{
			new_string[new_index] = input_string[old_index];
		}
		new_string[number_of_byte_to_copy] = '\0';
		return new_string;
	}
	else
	{
		return "error";
	}
} 

//Set строки нулевыми символами
void mstring_set_null(char* string, int length)
{
	for (int i = 0; i < length; i++)
	{
		string[i] = '\0';
	}
}

char* mstring_concat(char* first, const char* second)
{
	size_t first_length = strlen(first);
	size_t second_length = strlen(second);
	size_t new_string_length = first_length + second_length;
	
	char* new_string = (char*) malloc((new_string_length+1)*sizeof(char));
	memcpy(new_string, first, first_length);
	memcpy(new_string+first_length, second, (second_length+1));
	
	return new_string;
}

char* mstring_concat_char(char* first, char second)
{
	size_t first_length = strlen(first);
	size_t new_string_length = first_length + 1;
	
	char* new_string = (char*) malloc((first_length + 2)*sizeof(char));
	
	if (first_length > 0) memcpy(new_string, first, first_length);
	new_string[first_length] = second;
	new_string[new_string_length] = '\0';
	
	return new_string;
}

char* mstring_concat_chars(char first, char second)
{
	char* new_string = (char*) malloc(3*sizeof(char));
	
	new_string[0] = first;
	new_string[1] = second;
	new_string[2] = '\0';
	
	return new_string;
}

char* mstring_convert_int32_to_string(int32_t number_to_convert)
{
	char* converted_int32 = (char*) malloc(10*sizeof(char));
	sprintf(converted_int32, "%d", number_to_convert); 
	return converted_int32;
} 