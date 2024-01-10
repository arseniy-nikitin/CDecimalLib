#include "../../s21_decimal.h"

void bank_round(s21_big_decimal *dec, uint8_t remainder) {
  s21_big_decimal buf = {0};
  uint8_t flag = 0;
  if (IS_BIG_MANTISSA_TRUE(*dec))
    flag = get_highest_dec_digit(division_int(*dec, BIG_DIGIT, &buf)) % 2;
  else
    flag = 1;
  if (flag && (remainder >= 5)) addition(*dec, BIG_UNIT, dec);
}
