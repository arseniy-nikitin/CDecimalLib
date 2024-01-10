#include "../s21_decimal.h"

/*
Округляет Decimal до ближайшего целого числа.
*/
int s21_round(s21_decimal dec, s21_decimal *res) {
  int error = 0;
  for (int i = LOWEST_DIGIT; i <= FUNC_ELEM; i++) {  // Зануляем децимал
    res->bits[i] = 0;
  }
  double tmp_double = 0.0;
  if (get_scale(dec)) {
    error = s21_from_decimal_to_double(dec, &tmp_double);
    tmp_double = round(tmp_double);
    // Переводим обратно в децимал
    if (S21_IS_NEGATIVE(dec)) {
      set_bit(&res->bits[FUNC_ELEM], MINUS_BIT);
      tmp_double *= -1;
    }
    fill_mantissa(res, tmp_double);
  } else {
    for (uint8_t i = LOWEST_DIGIT; i <= FUNC_ELEM; i++) {
      res->bits[i] = dec.bits[i];
    }
  }
  return error;
}
