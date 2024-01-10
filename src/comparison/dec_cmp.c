#include "../s21_decimal.h"

int comparison(s21_decimal dec1, s21_decimal dec2) {
  int result = 0;
  int negative = 0;

  uint8_t scale_1 = get_scale(dec1), scale_2 = get_scale(dec2);
  int diff_in_scale = who_is_bigger(scale_1, scale_2);

  result = who_is_bigger(!S21_IS_NEGATIVE(dec1), !S21_IS_NEGATIVE(dec2));

  if (S21_IS_NEGATIVE(dec1) && S21_IS_NEGATIVE(dec2)) negative = 1;

  if (diff_in_scale == 1)
    shift_denominators(&dec2, &dec1);
  else if (diff_in_scale == 2)
    shift_denominators(&dec1, &dec2);

  if (result == 0) {
    if (S21_IS_INF(dec1) && !S21_IS_INF(dec2)) {
      result = 1;
    } else if (S21_IS_INF(dec2) && !S21_IS_INF(dec1)) {
      result = 2;
    } else {
      result = bits_cmp(dec1, dec2);
    }
    if (negative && result != 0) {
      result = (result == 1 ? 2 : 1);
    }
  } else {
    if (!S21_IS_TRUE(dec1) && !S21_IS_TRUE(dec2)) {
      result = 0;
    }
  }
  return result;
}

int who_is_bigger(int num1, int num2) {
  int result = 0;
  if (num1 > num2)
    result = 1;
  else if (num2 > num1)
    result = 2;
  else if (num1 == num2)
    result = 0;
  return result;
}

int bits_cmp(s21_decimal dec1, s21_decimal dec2) {
  int result = 0;
  int flag = 0;
  for (int i = HIGHEST_DIGIT; !flag && i >= LOWEST_DIGIT; i--) {
    if (dec1.bits[i] > dec2.bits[i]) {
      result = 1;
      flag = 1;
    } else if (dec1.bits[i] < dec2.bits[i]) {
      result = 2;
      flag = 1;
    }
  }
  return result;
}