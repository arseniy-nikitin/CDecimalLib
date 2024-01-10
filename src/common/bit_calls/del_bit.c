#include "../../s21_decimal.h"

void del_bit(uint32_t *val, uint8_t shift) { *val &= ~(1U << shift); }