//8-Bit Binary Numbers

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

uint32_t Bits2Unsigned(int8_t bits[8]);

int32_t Bits2Signed(int8_t bits[8])
{
	//Convert array of bits to signed int
	int32_t sum = 0;
	int32_t sum2 = 0;
	if (bits[7] == 0) //If the most significant bit is a 0, summation of all the other bits
	{
		for (int i = 6; i >= 0; i--)
		{
			sum = 2*sum +bits[i];
		}
		return sum;
	}
	else if (bits[7] == 1) //If the most significant bit is a 1, set sum = -128, add all other bits and add it to -128
	{
		sum = -128;
		for (int i = 6; i >= 0; i--)
			sum2 = 2*sum2 + bits[i];
		sum = sum + sum2;
		return sum;
	}
	return sum;
}

uint32_t Bits2Unsigned(int8_t bits[8])
{
	//Convert array of bits to unsigned int
	uint32_t sum = 0;
	for (int i = 7; i >= 0; i--)//Goes from the most significant bit, and multiply by two for each bit
	{
		sum = 2*sum +bits[i];
	}
	return sum;
}

void Increment(int8_t bits[8])
{
	//Add 1 to value represented by bit pattern
	for (int i = 0; i < 8; i++)
	{
		if (bits[i] == 0 && i == 1)//If the least significant bit is equal to zero, make it 1 and break
		{
			bits[i] = 1;
			return;
		}
		else if (bits[i] == 1)//If the bit is 1, set it to zero and go to the next bit
			bits[i] = 0;
		else if (bits[i] == 0)//If the bit is 0, set it to 1 and break since it has successfully incremented
		{
			bits[i] = 1;
			return;
		}
	}
}

void Unsigned2Bits(uint32_t n, int8_t bits[8])
{
	//Opposite of Bits2Unsigned
	int count = 0;
	if (n == 0)//If the number is zero, set all bits of the array to zero
	{
		for (int i = 0; i < 8; i++)
			bits[i] = 0;
	}
	while (n != 0) // Mod the number, set the remainder to the bit and do repeated division
	{
		bits[count] = n % 2;
		count++;
		n = n / 2;
	}
}
