#pragma once

// refs https://en.wikipedia.org/wiki/Q_(number_format)
// refs http://www.atmel.com/images/doc1631.pdf
// results of mul_q*_q* are approximated

#include "common.h"

INLINE uint16_t mul_q16_q16(uint16_t x, uint16_t y) {
  uint16_t result  = high_byte(low_byte(x) * high_byte(y));
  result          += high_byte(high_byte(x) * low_byte(y));
  result          += high_byte(x) * high_byte(y);
  return result;
}

INLINE int16_t mul_q16s_q16s(int16_t x, int16_t y) {
  int16_t result  = high_sbyte(low_byte(x) * high_sbyte(y));
  result         += high_sbyte(high_sbyte(x) * low_byte(y));
  result         += high_sbyte(x) * high_sbyte(y);
  return result;
}

INLINE int16_t mul_q16s_q16(int16_t x, uint16_t y) {
  uint16_t result  = high_byte(low_byte(x) * high_byte(y));
  result          += high_sbyte(high_sbyte(x) * low_byte(y));
  result          += high_sbyte(x) * high_byte(y);
  return result;
}

INLINE uint16_t mul_q16_q8(uint16_t x, uint8_t y) {
  uint16_t result  = high_byte(low_byte(x) * y);
  result          += high_byte(x) * y;
  return result;
}

INLINE int16_t mul_q16_q8s(uint16_t x, int8_t y) {
  int16_t result  = high_sbyte(low_byte(x) * y);
  result         += high_byte(x) * y;
  return result;
}

INLINE int16_t mul_q16s_q8(int16_t x, uint8_t y) {
  int16_t result  = high_byte(low_byte(x) * y);
  result         += high_sbyte(x) * y;
  return result;
}

INLINE int16_t mul_q16s_q8s(int16_t x, int8_t y) {
  int16_t result  = high_sbyte(low_byte(x) * y);
  result         += high_sbyte(x) * y;
  return result;
}
