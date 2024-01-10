#include "../s21_decimal.h"

/*
Округляет указанное Decimal число до ближайшего целого числа в сторону
отрицательной бесконечности.
*/
int s21_floor(s21_decimal dec, s21_decimal *res) {
  for (int i = LOWEST_DIGIT; i <= FUNC_ELEM; i++) {  // Зануляем децимал
    res->bits[i] = 0;
  }
  int error = 0;
  uint8_t scale = get_scale(dec);
  error = s21_truncate(dec, res);

  if (S21_IS_NEGATIVE(*res) && !get_scale(*res)) {
    // вычесть 1 из res
    s21_decimal one = {{1, 0, 0, 0}};
    if (scale) {
      s21_sub(*res, one, res);
    }
  }
  return error;
}
