#include "../../s21_decimal.h"

s21_big_decimal big_dec_not(s21_big_decimal res) {
  for (uint8_t i = LOWEST_BIG_DIGIT; i <= HIGHEST_BIG_DIGIT; i++)
    res.bits[i] = ~res.bits[i];
  return res;
}