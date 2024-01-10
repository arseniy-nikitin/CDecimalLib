#include "../s21_decimal.h"

int s21_is_equal(s21_decimal dec1, s21_decimal dec2) {
  int result = 0;
  if (comparison(dec1, dec2) == 0)
    result = 1;
  else
    result = 0;
  return result;
}
