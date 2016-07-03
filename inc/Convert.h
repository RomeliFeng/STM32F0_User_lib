/*
 * Convert.h
 *
 *  Created on: 2016Äê6ÔÂ30ÈÕ
 *      Author: Romeli
 */

#ifndef CONVERT_H_
#define CONVERT_H_

typedef enum
{
	AutoConvert = 0,
} ConvertMode;

char* numTostr(char* buf, long num, ConvertMode mode, unsigned char len = 0);
char* floatTostr(char* buf, float num, ConvertMode mode, unsigned char ndigit);
unsigned char getIntegerLen(long num);

#endif /* CONVERT_H_ */
