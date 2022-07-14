/**
 * @file utils.h
 * @author Brian Bradley (brian.bradley.p@gmail.com)
 * @brief 
 * @date 2022-07-11
 * 
 * @copyright Copyright (C) 2022 Brian Bradley
 * 
 * SPDX-License-Identifier: Apache-2.0
 * 
 */

#ifndef __THERMOLIB_UTILS_H
#define __THERMOLIB_UTILS_H

/* Evaluates to 0 if cond is true-ish; compile error otherwise */
#define ZERO_OR_COMPILE_ERROR(cond) ((int) sizeof(char[1 - 2 * !(cond)]) - 1)

/* Evaluates to 0 if array is an array; compile error if not array (e.g.
 * pointer)
 */
#define IS_ARRAY(array) \
	ZERO_OR_COMPILE_ERROR( \
		!__builtin_types_compatible_p(__typeof__(array), \
					      __typeof__(&(array)[0])))

/* Evaluates to number of elements in an array; compile error if not
 * an array (e.g. pointer)
 */
#define ARRAY_SIZE(array) \
	((long) (IS_ARRAY(array) + (sizeof(array) / sizeof((array)[0]))))

#define MIN(A,B)    ({ __typeof__(A) __a = (A); __typeof__(B) __b = (B); __a < __b ? __a : __b; })
#define MAX(A,B)    ({ __typeof__(A) __a = (A); __typeof__(B) __b = (B); __a < __b ? __b : __a; })

#define CLAMP(x, low, high) ({\
  __typeof__(x) __x = (x); \
  __typeof__(low) __low = (low);\
  __typeof__(high) __high = (high);\
  __x > __high ? __high : (__x < __low ? __low : __x);\
  })

#endif
