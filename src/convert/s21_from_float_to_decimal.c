#include "../s21_decimal.h"
int scale_processing(int integer_scale, const int scale_base,
                     int *integer_significant_number);
int tralling_zeros(int integer_significant_number, int integer_scale);
void writing_number_to_array(int integer_significant_number,
                             s21_decimal *bits_array, int scale_mul,
                             int integer_scale);
int proccesing_integer_number(int integer_significant_number,
                              int integer_scale);

int s21_from_float_to_decimal(float conversion_number,
                              s21_decimal *bits_array) {
  if (isnan(conversion_number) || isinf(conversion_number)) {
    return 1;
  }
  for (int i = LOWEST_DIGIT; i <= FUNC_ELEM; i++) {
    bits_array->bits[i] = 0;
  }
  char significant_number[100] = {0}, scale[100] = {0}, tmp_buffer[100] = {0};
  sprintf(tmp_buffer, "%e", conversion_number);
  int const max_number = 7922816, scale_base = 6;
  int index_scale = 0, sign = 0, index_full_number = 0, index_total_number = 0;
  if (tmp_buffer[sign] == '-') {
    sign = 1;
    set_bit(&bits_array->bits[FUNC_ELEM], MINUS_BIT);
  }
  int sign_flag = (sign == 0) ? 8 : 9;
  for (; tmp_buffer[index_full_number] != '\0';
       index_full_number++, index_total_number++) {
    if (index_full_number < sign_flag) {
      if (tmp_buffer[index_full_number] == '.') {
        index_full_number++;
      }
      significant_number[index_total_number] = tmp_buffer[index_full_number];
    } else if (index_full_number == sign_flag) {
      continue;
    } else if (index_full_number > sign_flag) {
      scale[index_scale] = tmp_buffer[index_full_number];
      index_scale++;
    }
  }
  int integer_significant_number = atoi(significant_number),
      integer_scale = atoi(scale), scale_mul = integer_scale;
  if (abs(integer_scale) > 28 ||
      (abs(integer_scale) == 28 &&
       abs(integer_significant_number) >= max_number)) {
    return 1;
  }
  if (conversion_number != 0.0) {
    if (integer_significant_number < 0) integer_significant_number *= -1;
    scale_mul -= scale_base;
    integer_scale = scale_processing(integer_scale, scale_base,
                                     &integer_significant_number);
    writing_number_to_array(integer_significant_number, bits_array, scale_mul,
                            integer_scale);
  }
  return 0;
}

int scale_processing(int integer_scale, const int scale_base,
                     int *integer_significant_number) {
  int tralling_zero = 0;
  if (integer_scale < 0) {
    integer_scale = abs(integer_scale) + scale_base;
    tralling_zero = tralling_zeros(*integer_significant_number, integer_scale);
  } else if (integer_scale >= 0 && integer_scale <= 6) {
    integer_scale = scale_base - integer_scale;
    if (integer_scale != 0) {
      tralling_zero =
          tralling_zeros(*integer_significant_number, integer_scale);
    }
  } else if (integer_scale > 6) {
    integer_scale = 0;
  }
  *integer_significant_number =
      proccesing_integer_number(*integer_significant_number, integer_scale);
  integer_scale -= tralling_zero;
  return integer_scale;
}
int proccesing_integer_number(int integer_significant_number,
                              int integer_scale) {
  int cycle_sift = tralling_zeros(integer_significant_number, integer_scale);
  while (cycle_sift > 0) {
    integer_significant_number /= 10;
    cycle_sift--;
  }
  return integer_significant_number;
}

int tralling_zeros(int integer_significant_number, int integer_scale) {
  int tralling_zero = 0;
  while (integer_significant_number && integer_significant_number % 10 == 0 &&
         tralling_zero != integer_scale) {
    tralling_zero++;
    integer_significant_number /= 10;
  }
  return tralling_zero;
}

void writing_number_to_array(int integer_significant_number,
                             s21_decimal *bits_array, int scale_mul,
                             int integer_scale) {
  int take_bit = 0;
  for (int index_bit = 0; index_bit <= LAST_BIT; index_bit++) {
    take_bit = integer_significant_number % 2;
    if (take_bit == 1) {
      set_bit(&bits_array->bits[LOWEST_DIGIT], index_bit);
    }
    integer_significant_number = integer_significant_number / 2;
  }
  set_scale(bits_array, integer_scale);

  if (scale_mul > 0) {
    while (scale_mul > 0) {
      s21_mul(*bits_array, DEC_DIGIT, bits_array);
      scale_mul--;
    }
  }
}
