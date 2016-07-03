/*
 * Convert.cpp
 *
 *  Created on: 2016Äê6ÔÂ30ÈÕ
 *      Author: Romeli
 */
#include "Convert.h"
#include "math.h"
#include "string.h"

char* numTostr(char* buf, long num, ConvertMode mode, unsigned char len)
{
	unsigned char index = 0;
	if (len == 0)
		len = getIntegerLen(num);
	if (num < 0)
	{
		buf[index] = '-';
		index++;
		num = -num;
	}
	switch (mode)
	{
	case AutoConvert:
		buf[len + index] = '\0';
		index = len - 1 + index;
		do
		{
			buf[index--] = num % 10 + '0';
			num /= 10;
		} while (--len);
		break;
	default:
		break;
	}
	return buf;
}

char* floatTostr(char* buf,float num, ConvertMode mode, unsigned char ndigit)
{
	char tmp[10];
	int aa=(int)num;
	float bb=num-aa;
	numTostr(buf,num,mode,0);
	numTostr(tmp,bb,mode,0);
	strcat(buf,tmp);
	return buf;
}

unsigned char getIntegerLen(long num)
{
	unsigned char len = 0;
	while (num <= -1 || num >= 1)
	{
		num /= 10;
		len++;
	}
	return len;
}

