#include "../s21_decimal.h"

/*
Возвращает целые цифры указанного Decimal числа; любые дробные цифры
отбрасываются, включая конечные нули.
*/
int s21_truncate(s21_decimal dec, s21_decimal *res) {
  for (int i = LOWEST_DIGIT; i <= FUNC_ELEM; i++) {  // Зануляем децимал
    res->bits[i] = 0;
  }
  int error = 0;
  uint8_t scale = get_scale(dec);
  if (!scale) {  // если число int
    for (uint8_t i = LOWEST_DIGIT; i <= FUNC_ELEM; i++) {
      res->bits[i] = dec.bits[i];
    }
  } else {  // обработка float
    double tmp_double = 0.0;
    error = s21_from_decimal_to_double(dec, &tmp_double);
    tmp_double = (long int)tmp_double;
    del_scale(res);
    // Переводим обратно в децимал
    if (S21_IS_NEGATIVE(dec)) {
      set_bit(&res->bits[FUNC_ELEM], MINUS_BIT);
      tmp_double *= -1;
    }
    fill_mantissa(res, tmp_double);
  }
  return error;
}

void fill_mantissa(s21_decimal *res, double tmp_double) {
  for (int i = LOWEST_DIGIT; i <= HIGHEST_DIGIT && tmp_double != 0; i++) {
    for (int j = FIRST_BIT; j <= LAST_BIT && tmp_double != 0; j++) {
      tmp_double = tmp_double / 2;
      if (tmp_double - (long int)tmp_double != 0) {
        set_bit(&res->bits[i], j);
      }
      tmp_double = (long int)tmp_double;
    }
  }
}
