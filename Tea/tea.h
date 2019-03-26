#pragma once

//#define INT_TEST

typedef struct DataStructure
{
	char string[8];
	int32_t i32[2];
	int64_t i64;
} Data;

//зашифровка
void encrypt_data(Data* value, int32_t* k, int8_t index)
{
    int32_t v0 = value[index].i32[0],
			v1 = value[index].i32[1],
			sum = 0,
			delta = 0x9e3779b9,
			ivec0 = 0x11234562,
			ivec1 = 0x11234562,
			n = 32;
    while (n-- > 0)
    {
        v0 += ((v1 << 4) + k[0]) ^ (v1 + sum) ^ ((v1 >> 5) + k[1]);
        sum += delta;
        v1 += ((v0 << 4) + k[2]) ^ (v0 + sum) ^ ((v0 >> 5) + k[3]);
    }
    value[index].i32[0] = (v0 | (ivec0 & (0xff>>8)));
    value[index].i32[1] = (v1 | (ivec1 & (0xff>>8)));
	value[index].i64 = (int64_t)v0;
	value[index].i64 = add_high_bytes_to_int64(value[index].i64, v1);
	char* str = convert_int64_to_char8(value[index].i64);
	memcpy((void*)value[index].string, (void*)str, 8);
}

//расшифровка
void decrypt_data(Data* value, int32_t* k, int8_t index)
{
	int32_t v0 = value[index].i32[0],
			v1 = value[index].i32[1],
			sum = 0xC6EF3720,
			ivec0 = 0x11234562,
			ivec1 = 0x11234562,
			delta = 0x9e3779b9,
			n = 32;
    while (n-- > 0)
    {
        v1 -= ((v0 << 4) + k[2]) ^ (v0 + sum) ^ ((v0 >> 5) + k[3]);
        sum -= delta;
        v0 -= ((v1 << 4) + k[0]) ^ (v1 + sum) ^ ((v1 >> 5) + k[1]);
    }
    value[index].i32[0] = (v0 | (ivec0 & (0xff>>8)));
    value[index].i32[1] = (v1 | (ivec1 & (0xff>>8)));
	value[index].i64 = (int64_t)v0;
	value[index].i64 = add_high_bytes_to_int64(value[index].i64, v1);
	char* str = convert_int64_to_char8(value[index].i64);
	memcpy((void*)value[index].string, (void*)str, 8);
}