#include "../../s21_decimal.h"

uint8_t get_bit(uint32_t val, uint8_t shift) { return !!(val & 1U << shift); }