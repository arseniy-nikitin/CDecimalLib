#include "../../s21_decimal.h"

void set_sign(s21_decimal *dec, uint8_t sign) {
  if (sign)
    set_bit(&dec->bits[FUNC_ELEM], MINUS_BIT);
  else
    del_bit(&dec->bits[FUNC_ELEM], MINUS_BIT);
}
