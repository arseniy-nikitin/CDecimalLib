#include "../s21_decimal.h"

int s21_is_less(s21_decimal dec1, s21_decimal dec2) {
  int result = 0;
  if (comparison(dec1, dec2) == 2)
    result = 1;
  else
    result = 0;
  return result;
}