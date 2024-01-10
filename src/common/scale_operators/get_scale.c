#include "../../s21_decimal.h"

uint8_t get_scale(s21_decimal dec) {
  uint8_t scale = 0;
  scale |= dec.bits[FUNC_ELEM] >> 16;
  return scale;
}
