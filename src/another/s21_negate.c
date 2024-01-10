#include "../s21_decimal.h"

int s21_negate(s21_decimal dec, s21_decimal *res) {
  swap_bit(&dec.bits[FUNC_ELEM], LAST_BIT);
  res->bits[FUNC_ELEM] = dec.bits[FUNC_ELEM];
  for (uint8_t i = LOWEST_DIGIT; i <= HIGHEST_DIGIT; i++)
    res->bits[i] = dec.bits[i];
  return 0;
}