#include "../s21_decimal.h"

void process_overflow(s21_big_decimal *dec, uint8_t *scale);
void process_small(s21_big_decimal *dec, uint8_t *scale);
uint8_t get_value_mul(s21_big_decimal big_dec, uint8_t scale, uint8_t sign,
                      s21_decimal *res);

int s21_mul(s21_decimal dec1, s21_decimal dec2, s21_decimal *res) {
  if (S21_IS_NAN(dec1) || S21_IS_NAN(dec2)) {
    *res = S21_IS_NAN(dec1) ? dec1 : dec2;
    return 3;
  }
  uint8_t sign = S21_IS_NEGATIVE(dec1) ^ S21_IS_NEGATIVE(dec2);
  uint8_t scale = get_scale(dec1) + get_scale(dec2);
  if (!IS_MANTISSA_TRUE(dec1) || !IS_MANTISSA_TRUE(dec2)) {
    *res = !IS_MANTISSA_TRUE(dec1) ? dec1 : dec2;
    set_sign(res, sign);
    set_scale(res, scale);
    return 0;
  }
  if (S21_IS_INF(dec1) || S21_IS_INF(dec2)) {
    *res = S21_IS_INF(dec1) ? dec1 : dec2;
    set_sign(res, sign);
    return S21_IS_NEGATIVE(*res) ? 2 : 1;
  }
  s21_big_decimal big_res = TO_BIG(*res);
  if (multiplication(TO_BIG(dec1), TO_BIG(dec2), &big_res))
    process_overflow(&big_res, &scale);
  if (scale > MAX_SCALE && scale != INF_SCALE) process_small(&big_res, &scale);
  uint8_t code = get_value_mul(big_res, scale, sign, res);
  return code;
}

uint8_t multiplication(s21_big_decimal dec1, s21_big_decimal dec2,
                       s21_big_decimal *res) {
  s21_big_decimal buf = {0};
  while (IS_BIG_MANTISSA_TRUE(dec2)) {
    if (get_bit(dec2.bits[LOWEST_BIG_DIGIT], FIRST_BIT))
      addition(buf, dec1, &buf);
    dec1 = big_dec_left(dec1, 1);
    dec2 = big_dec_right(dec2, 1);
  }
  *res = buf;
  return IS_OVERFLOW(*res);
}

void process_overflow(s21_big_decimal *dec, uint8_t *scale) {
  s21_big_decimal remainder = {0};
  while (IS_OVERFLOW(*dec) && *scale) {
    remainder = division_int(*dec, BIG_DIGIT, dec);
    (*scale)--;
  }
  if (IS_OVERFLOW(*dec))
    *scale = INF_SCALE;
  else
    bank_round(dec, get_highest_dec_digit(remainder));
}

void process_small(s21_big_decimal *dec, uint8_t *scale) {
  s21_big_decimal remainder = {0};
  while (*scale > MAX_SCALE) {
    remainder = division_int(*dec, BIG_DIGIT, dec);
    (*scale)--;
  }
  bank_round(dec, get_highest_dec_digit(remainder));
}

uint8_t get_value_mul(s21_big_decimal big_dec, uint8_t scale, uint8_t sign,
                      s21_decimal *res) {
  *res = TO_NORMAL(big_dec);
  set_sign(res, sign);
  set_scale(res, scale);
  if (scale == INF_SCALE) {
    DEL_MANTISSA(*res);
    return S21_IS_NEGATIVE(*res) ? 2 : 1;
  }
  return 0;
}
