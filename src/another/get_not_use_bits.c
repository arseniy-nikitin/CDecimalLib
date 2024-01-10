#include "../s21_decimal.h"

int get_not_use_bits(s21_decimal bits_array) {
  for (int index_one = 24, index_two = 15; index_one <= 30 && index_two >= 0;
       index_one++, index_two--) {
    int value_for_check_zero_index_one =
        get_bit(bits_array.bits[FUNC_ELEM], index_one);
    int value_for_check_zero_index_two =
        get_bit(bits_array.bits[FUNC_ELEM], index_two);
    if (value_for_check_zero_index_one != 0 &&
        value_for_check_zero_index_two != 0) {
      return 1;
    } else {
      return 0;
    }
  }
  return 0;
}