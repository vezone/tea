#pragma once

//#define INT_TEST

//Структура для работы с шифром
typedef union DataUnion
{
	char     string[8];
	uint32_t u32ab[2];
	uint64_t u64ab;
} Data;

//зашифровка
void EncryptData(Data* value, uint32_t* k)
{
    uint32_t v0 = value->u32ab[0],
             v1 = value->u32ab[1],
             sum = 0,
             delta = 0x9e3779b9,
             n = 32;
    while (n-- > 0)
    {
        v0 += ((v1 << 4) + k[0]) ^ (v1 + sum) ^ ((v1 >> 5) + k[1]);
        sum += delta;
        v1 += ((v0 << 4) + k[2]) ^ (v0 + sum) ^ ((v0 >> 5) + k[3]);
    }
    value->u32ab[0] = v0;
    value->u32ab[1] = v1;
}

//расшифровка
void DecryptData(Data* value, uint32_t* k)
{
    uint32_t v0 = value->u32ab[0],
             v1 = value->u32ab[1],
             sum   = 0xC6EF3720,
             delta = 0x9e3779b9,
             n = 32;
      
    while (n-- > 0)
    {
        v1 -= ((v0 << 4) + k[2]) ^ (v0 + sum) ^ ((v0 >> 5) + k[3]);
        sum -= delta;
        v0 -= ((v1 << 4) + k[0]) ^ (v1 + sum) ^ ((v1 >> 5) + k[1]);
    }
    value->u32ab[0] = v0;
    value->u32ab[1] = v1;
}

void TeaIntTest(Data value, uint32_t* key)
{	
	EncryptData(&value, key);
	g_print("Encrypted int = %lu\n", value.u64ab);
    
    DecryptData(&value, key);
	g_print("Decrypted int = %lu\n", value.u64ab);
	
}

void TeaStringTest(Data value, uint32_t* key)
{
	EncryptData(&value, key);
    g_print("Encrypted string = %s\n", value.string); 
    
    DecryptData(&value, key);
    g_print("decrypted string = %s\n", value.string); 
}