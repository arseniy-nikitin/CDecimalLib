#include "../s21_decimal.h"

int s21_from_int_to_decimal(int conversion_number, s21_decimal *bits_array) {
  for (int i = LOWEST_DIGIT; i <= FUNC_ELEM; i++) {  // Зануляем децимал
    bits_array->bits[i] = 0;
  }
  int index = 0;
  uint32_t un_integer_tmp = 0;
  int binary[LAST_BIT] = {0};
  if (conversion_number == INT_MIN) {
    un_integer_tmp = -conversion_number;
  } else if (conversion_number < 0) {
    set_sign(bits_array, 1);
    conversion_number = conversion_number * (-1);
    un_integer_tmp = conversion_number;
  } else {
    un_integer_tmp = conversion_number;
  }
  if (conversion_number < 0) {
    set_sign(bits_array, 1);
    conversion_number = conversion_number * (-1);
  }
  for (; un_integer_tmp > 0; index++) {
    binary[index] = un_integer_tmp % 2;
    un_integer_tmp = un_integer_tmp / 2;
  }
  for (int transfer_index = 0; transfer_index < index; transfer_index++) {
    if (binary[transfer_index] == 1) {
      set_bit(&bits_array->bits[LOWEST_DIGIT], transfer_index);
    }
  }
  return 0;
}
