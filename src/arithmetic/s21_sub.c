#include "../s21_decimal.h"

uint8_t process_sign(s21_decimal *dec1, s21_decimal *dec2);
uint8_t process_scale_sub(s21_decimal *dec1, s21_decimal *dec2);
void get_value_sub(s21_big_decimal big_dec, uint8_t scale, uint8_t sign,
                   s21_decimal *res);

int s21_sub(s21_decimal dec1, s21_decimal dec2, s21_decimal *res) {
  if (S21_IS_NAN(dec1) || S21_IS_NAN(dec2)) {
    *res = S21_IS_NAN(dec1) ? dec1 : dec2;
    return 3;
  }
  swap_bit(&dec2.bits[FUNC_ELEM], LAST_BIT);
  if (!(S21_IS_NEGATIVE(dec1) ^ S21_IS_NEGATIVE(dec2)))
    return s21_add(dec1, dec2, res);
  uint8_t sign = process_sign(&dec1, &dec2);
  if (S21_IS_INF(dec1) && S21_IS_INF(dec2)) return 3;
  if (S21_IS_INF(dec1) || S21_IS_INF(dec2)) {
    *res = S21_IS_INF(dec1) ? dec1 : dec2;
    return S21_IS_NEGATIVE(*res) ? 2 : 1;
  }
  if (!S21_IS_TRUE(dec1) || !S21_IS_TRUE(dec2)) {
    *res = S21_IS_TRUE(dec1) ? dec1 : dec2;
    return 0;
  }
  uint8_t scale = process_scale_sub(&dec1, &dec2);
  s21_big_decimal big_res = TO_BIG(*res);
  subtraction(TO_BIG(dec1), TO_BIG(dec2), &big_res);
  get_value_sub(big_res, scale, sign, res);
  return 0;
}

uint8_t process_sign(s21_decimal *dec1, s21_decimal *dec2) {
  s21_decimal buf = {0};
  uint8_t sign = 0;
  if (s21_is_less(dec_abs(*dec1), dec_abs(*dec2))) {
    buf = *dec1;
    *dec1 = *dec2;
    *dec2 = buf;
  }
  if (S21_IS_NEGATIVE(*dec1)) sign = 1;
  return sign;
}

uint8_t process_scale_sub(s21_decimal *dec1, s21_decimal *dec2) {
  if (get_scale(*dec1) < get_scale(*dec2))
    shift_denominators(dec1, dec2);
  else if (get_scale(*dec1) > get_scale(*dec2))
    shift_denominators(dec2, dec1);
  return get_scale(*dec1);
}

void subtraction(s21_big_decimal dec1, s21_big_decimal dec2,
                 s21_big_decimal *res) {
  s21_big_decimal buf = {0};
  while (IS_BIG_MANTISSA_TRUE(dec2)) {
    big_dec_and(big_dec_not(dec1), dec2, &buf);
    big_dec_xor(dec1, dec2, &dec1);
    buf = big_dec_left(buf, 1);
    dec2 = buf;
  }
  *res = dec1;
}

void get_value_sub(s21_big_decimal big_dec, uint8_t scale, uint8_t sign,
                   s21_decimal *res) {
  *res = TO_NORMAL(big_dec);
  set_sign(res, sign);
  set_scale(res, scale);
}
