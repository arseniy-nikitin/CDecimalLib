#include "../../s21_decimal.h"

void big_dec_and(s21_big_decimal dec1, s21_big_decimal dec2,
                 s21_big_decimal *res) {
  for (uint8_t i = LOWEST_BIG_DIGIT; i <= HIGHEST_BIG_DIGIT; i++)
    res->bits[i] = dec1.bits[i] & dec2.bits[i];
}