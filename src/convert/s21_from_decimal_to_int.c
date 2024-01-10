#include "../s21_decimal.h"

int s21_from_decimal_to_int(s21_decimal bits_array, int *conversion_number) {
  if (conversion_number == NULL) {
    return 1;
  } else {
    *conversion_number = 0;
  }
  int sign = get_bit(bits_array.bits[FUNC_ELEM], LAST_BIT);
  int scale = get_scale(bits_array);
  s21_big_decimal buf = TO_BIG(bits_array);
  while (scale > 0) {
    division_int(buf, BIG_DIGIT, &buf);
    scale--;
  }
  bits_array = TO_NORMAL(buf);
  set_sign(&bits_array, sign);
  if (bits_array.bits[MIDDLE_DIGIT] == 0 &&
      bits_array.bits[HIGHEST_DIGIT] == 0) {
    int index = 0;
    for (; index <= LAST_BIT; index++) {
      int current_bit = get_bit(bits_array.bits[LOWEST_DIGIT], index);
      *conversion_number += current_bit * pow(2, index);
    }
  } else {
    return 1;
  }
  if (sign > 0) {
    *conversion_number *= -1;
  }
  return 0;
}
