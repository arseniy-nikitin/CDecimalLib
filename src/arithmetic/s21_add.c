#include "../s21_decimal.h"

uint8_t process_scale_add(s21_decimal *dec1, s21_decimal *dec2);
uint8_t get_value_add(s21_big_decimal big_dec, uint8_t scale, uint8_t sign,
                      s21_decimal *res);

int s21_add(s21_decimal dec1, s21_decimal dec2, s21_decimal *res) {
  if (S21_IS_NAN(dec1) || S21_IS_NAN(dec2)) {
    *res = S21_IS_NAN(dec1) ? dec1 : dec2;
    return 3;
  }
  if (S21_IS_NEGATIVE(dec1) ^ S21_IS_NEGATIVE(dec2)) {
    s21_negate(dec2, &dec2);
    return s21_sub(dec1, dec2, res);
  }
  if (S21_IS_INF(dec1) || S21_IS_INF(dec2)) {
    *res = S21_IS_INF(dec1) ? dec1 : dec2;
    return S21_IS_NEGATIVE(*res) ? 2 : 1;
  }
  if (!S21_IS_TRUE(dec1) || !S21_IS_TRUE(dec2)) {
    *res = S21_IS_TRUE(dec1) ? dec1 : dec2;
    return 0;
  }
  uint8_t sign = S21_IS_NEGATIVE(dec1);
  uint8_t scale = process_scale_add(&dec1, &dec2);
  s21_big_decimal big_res = TO_BIG(*res);
  if (addition(TO_BIG(dec1), TO_BIG(dec2), &big_res))
    if (scale--)
      bank_round(&big_res, get_highest_dec_digit(
                               division_int(big_res, BIG_DIGIT, &big_res)));
  uint8_t code = get_value_add(big_res, scale, sign, res);
  return code;
}

uint8_t process_scale_add(s21_decimal *dec1, s21_decimal *dec2) {
  if (get_scale(*dec1) < get_scale(*dec2))
    shift_denominators(dec1, dec2);
  else if (get_scale(*dec1) > get_scale(*dec2))
    shift_denominators(dec2, dec1);
  return get_scale(*dec1);
}

uint8_t addition(s21_big_decimal dec1, s21_big_decimal dec2,
                 s21_big_decimal *res) {
  s21_big_decimal buf = {0};
  while (IS_BIG_MANTISSA_TRUE(dec2)) {
    big_dec_and(dec1, dec2, &buf);
    big_dec_xor(dec1, dec2, &dec1);
    buf = big_dec_left(buf, 1);
    dec2 = buf;
  }
  *res = dec1;
  return IS_OVERFLOW(*res);
}

uint8_t get_value_add(s21_big_decimal big_dec, uint8_t scale, uint8_t sign,
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
