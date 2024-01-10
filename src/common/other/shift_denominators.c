#include "../../s21_decimal.h"

void shift_denominators(s21_decimal *lower, s21_decimal *higher) {
  uint8_t exp_low = get_scale(*lower);
  uint8_t exp_high = get_scale(*higher);
  s21_big_decimal buf = TO_BIG(*lower);
  while (exp_low != exp_high && !IS_OVERFLOW(buf)) {
    if (!multiplication(buf, BIG_DIGIT, &buf)) {
      *lower = TO_NORMAL(buf);
      exp_low++;
    }
  }
  s21_big_decimal remainder = {0};
  buf = TO_BIG(*higher);
  while (exp_high != exp_low) {
    remainder = division_int(buf, BIG_DIGIT, &buf);
    exp_high--;
  }
  bank_round(&buf, get_highest_dec_digit(remainder));
  *higher = TO_NORMAL(buf);
  set_scale(lower, exp_low);
  set_scale(higher, exp_high);
}
