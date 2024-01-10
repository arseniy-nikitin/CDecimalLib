#include "../s21_decimal.h"

int s21_is_not_equal(s21_decimal dec1, s21_decimal dec2) {
  int result = 0;
  result = comparison(dec1, dec2);
  if (result != 0 && result != -1)
    result = 1;
  else
    result = 0;
  return result;
}