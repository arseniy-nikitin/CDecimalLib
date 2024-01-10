#include "../../s21_decimal.h"

void del_scale(s21_decimal *dec) {
  dec->bits[FUNC_ELEM] &= ~(0b11111111U << 16);
}
