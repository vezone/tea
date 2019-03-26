#pragma once

void data_compression(/*char* input_data*/)
{
	char* input_data = 
		//"www.yandex.com";
		"wwwwwwwwweeeeeeee222211sawaw";
	int input_data_length = strlen(input_data) - 1;
	
	char* output = (char*) malloc((input_data_length+1)*sizeof(char));
	output = "";
	char* temp;
	int temp_counter = 0;
	
	int i, j;
	for (i = 0; i <= input_data_length; i++)
	{
		char temp_character = input_data[i];
		++temp_counter;
		for (j = (i+1); j <= (input_data_length+1); j++)
		{
			if (temp_character == input_data[j] ||
				i > input_data_length)
			{
				++temp_counter;
				
			}
			else 
			{
				temp = mstring_concat_char(mstring_convert_int32_to_string(temp_counter), temp_character);
				i += (temp_counter-1);
				temp_counter = 0;
				break;
			}
		}
		
		output = mstring_concat(output, temp); 
		
	}
	
	printf("output = %s\n", output);
	
}