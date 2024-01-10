#include "../../s21_decimal.h"

uint8_t get_highest_dec_digit(s21_big_decimal dec) {
  s21_big_decimal remainder = {0};
  while (IS_BIG_MANTISSA_TRUE(dec))
    remainder = division_int(dec, BIG_DIGIT, &dec);
  return remainder.bits[LOWEST_BIG_DIGIT];
}
