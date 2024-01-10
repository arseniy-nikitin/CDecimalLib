#include "../s21_decimal.h"

int s21_from_decimal_to_float(s21_decimal bits_array,
                              float *conversion_number) {
  if (conversion_number == NULL) {
    return 1;
  } else {
    *conversion_number = 0;
  }
  if (get_not_use_bits(bits_array) != 0) {
    return 1;
  }
  int sign = get_bit(bits_array.bits[FUNC_ELEM], LAST_BIT);
  double mantissa = convert_mantissa_to_double(bits_array);
  int scale = get_scale(bits_array);
  if (scale > 28 || scale < 0) {
    return 1;
  }
  while (scale > 0) {
    mantissa = mantissa / 10;
    scale--;
  }
  if (sign) {
    mantissa *= -1;
  }
  *conversion_number = mantissa;
  return 0;
}

double convert_mantissa_to_double(s21_decimal bits_array) {
  double result = 0;
  for (int index_arrays = HIGHEST_DIGIT; index_arrays >= LOWEST_DIGIT;
       index_arrays--) {
    for (int index_bits = LAST_BIT; index_bits >= 0; --index_bits) {
      result =
          result * 2.0 + get_bit(bits_array.bits[index_arrays], index_bits);
    }
  }
  return result;
}