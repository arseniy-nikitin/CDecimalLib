#include "../../s21_decimal.h"

void swap_bit(uint32_t *val, uint8_t shift) {
  if (get_bit(*val, shift))
    del_bit(val, shift);
  else
    set_bit(val, shift);
}