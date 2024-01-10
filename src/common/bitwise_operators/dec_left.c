#include "../../s21_decimal.h"

s21_big_decimal big_dec_left(s21_big_decimal dec, uint8_t shift) {
  while (shift--) {
    uint8_t overflow = 0;
    for (uint8_t i = LOWEST_BIG_DIGIT; i <= HIGHEST_BIG_DIGIT; i++) {
      uint8_t bit = get_bit(dec.bits[i], LAST_BIT);
      dec.bits[i] <<= 1U;
      if (overflow) set_bit(&dec.bits[i], FIRST_BIT);
      overflow = bit;
    }
  }
  return dec;
}