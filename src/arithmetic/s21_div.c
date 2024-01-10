#include "../s21_decimal.h"

uint8_t big_less_or_eq(s21_big_decimal dec1, s21_big_decimal dec2);
void process_scale(s21_big_decimal *dec, int16_t *scale);
uint8_t get_value_div(s21_big_decimal big_dec, uint8_t scale, uint8_t sign,
                      s21_decimal *res);

int s21_div(s21_decimal dec1, s21_decimal dec2, s21_decimal *res) {
  if (S21_IS_NAN(dec1) || S21_IS_NAN(dec2)) {
    *res = S21_IS_NAN(dec1) ? dec1 : dec2;
    return 3;
  }
  if (!IS_MANTISSA_TRUE(dec2)) {
    SET_NAN(*res);
    return 3;
  }
  if (S21_IS_INF(dec1) && S21_IS_INF(dec2)) return 3;
  if (!IS_MANTISSA_TRUE(dec1)) {
    *res = dec1;
    return 0;
  }
  uint8_t sign = S21_IS_NEGATIVE(dec1) ^ S21_IS_NEGATIVE(dec2);
  int16_t scale = get_scale(dec1) - get_scale(dec2);
  if (S21_IS_INF(dec1)) {
    *res = dec1;
    set_sign(res, sign);
    return 0;
  }
  if (S21_IS_INF(dec2)) {
    *res = (s21_decimal){0};
    set_sign(res, sign);
    return 0;
  }
  s21_big_decimal big_res = TO_BIG(*res);
  s21_big_decimal remainder =
      division_int(TO_BIG(dec1), TO_BIG(dec2), &big_res);
  division_fract(remainder, TO_BIG(dec2), &big_res, &scale);
  process_scale(&big_res, &scale);
  uint8_t code = get_value_div(big_res, (uint8_t)scale, sign, res);
  return code;
}

s21_big_decimal division_int(s21_big_decimal dec1, s21_big_decimal dec2,
                             s21_big_decimal *res) {
  s21_big_decimal buf = {0};
  int32_t quotient = 0;
  do {
    quotient = 0;
    s21_big_decimal carry = dec2;
    while (big_less_or_eq(carry, dec1))
      multiplication(dec2, big_dec_left(BIG_UNIT, quotient++), &carry);
    if (--quotient > 0) {
      addition(buf, big_dec_left(BIG_UNIT, quotient - 1), &buf);
      multiplication(dec2, big_dec_left(BIG_UNIT, quotient - 1), &carry);
      subtraction(dec1, carry, &dec1);
    }
  } while (quotient >= 0);
  *res = buf;
  return dec1;
}

void division_fract(s21_big_decimal rem, s21_big_decimal dec,
                    s21_big_decimal *res, int16_t *scale) {
  s21_big_decimal buf = *res;
  while (IS_BIG_MANTISSA_TRUE(rem) && !IS_OVERFLOW(buf) && *scale < MAX_SCALE) {
    multiplication(buf, BIG_DIGIT, &buf);
    if (!IS_OVERFLOW(buf) && *scale < MAX_SCALE) {
      s21_big_decimal num = {0};
      multiplication(rem, BIG_DIGIT, &rem);
      s21_big_decimal rem_buf = division_int(rem, dec, &num);
      if (IS_BIG_MANTISSA_TRUE(num)) rem = rem_buf;
      addition(buf, num, &buf);
      *res = buf;
      (*scale)++;
    }
  }
  bank_round(res, get_highest_dec_digit(rem));
}

uint8_t big_less_or_eq(s21_big_decimal dec1, s21_big_decimal dec2) {
  for (int8_t i = HIGHEST_BIG_DIGIT; i >= LOWEST_BIG_DIGIT; i--)
    for (int8_t n = LAST_BIT; n >= FIRST_BIT; n--)
      if (get_bit(dec1.bits[i], n) && !get_bit(dec2.bits[i], n))
        return 0;
      else if (!get_bit(dec1.bits[i], n) && get_bit(dec2.bits[i], n))
        return 1;
  return 1;
}

void process_scale(s21_big_decimal *dec, int16_t *scale) {
  while (*scale < 0 && !IS_OVERFLOW(*dec)) {
    multiplication(*dec, BIG_DIGIT, dec);
    (*scale)++;
  }
  if (IS_OVERFLOW(*dec)) *scale = INF_SCALE;
}

uint8_t get_value_div(s21_big_decimal big_dec, uint8_t scale, uint8_t sign,
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
