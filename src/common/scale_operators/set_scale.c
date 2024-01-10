#include "../../s21_decimal.h"

void set_scale(s21_decimal *dec, uint32_t scale) {
  del_scale(dec);
  dec->bits[FUNC_ELEM] |= scale << 16;
}
