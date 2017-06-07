/*
 * Parse.h
 *
 *  Created on: 2016��10��6��
 *      Author: Romeli
 */

#ifndef PARSE_H_
#define PARSE_H_

#include "cmsis_device.h"

class Parse {
public:
	static uint8_t getLen(uint32_t num, uint8_t base);
	static double pow10(uint8_t power);
	static uint8_t strcat(uint8_t* str_to, uint8_t str_to_len,
			uint8_t* str_from, uint8_t str_from_len);

	static uint8_t byNumber(uint32_t num, uint8_t base, uint8_t* str);
	static inline uint8_t byNumber(uint16_t num, uint8_t base, uint8_t* str) {
		return byNumber((uint32_t) num, base, str);
	}
	static inline uint8_t byNumber(uint8_t num, uint8_t base, uint8_t* str) {
		return byNumber((uint32_t) num, base, str);
	}
	static uint8_t byNumber(int32_t num, uint8_t base, uint8_t* str);
	static inline void byNumber(int16_t num, uint8_t base, uint8_t* str) {
		byNumber((int32_t) num, base, str);
	}
	static inline void byNumber(int8_t num, uint8_t base, uint8_t* str) {
		byNumber((int32_t) num, base, str);
	}

	static uint8_t byFloat(double flo, uint8_t ndigit, uint8_t* str);
	static inline uint8_t byFloat(float flo, uint8_t ndigit, uint8_t* str) {
		return byFloat((double) flo, ndigit, str);
	}
};

#endif /* PARSE_H_ */
