#include "../../s21_decimal.h"

s21_decimal dec_abs(s21_decimal dec) {
  del_bit(&dec.bits[FUNC_ELEM], LAST_BIT);
  return dec;
}