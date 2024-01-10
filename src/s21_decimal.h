#ifndef S21_DECIMAL
#define S21_DECIMAL

#include <limits.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/*
  DECIMAL TYPES
*/

typedef struct {
  unsigned int bits[4];
} s21_decimal;

typedef struct {
  unsigned int bits[7];
} s21_big_decimal;

/*
  CONSTANTS
*/

#define FIRST_BIT 0
#define LAST_BIT 31
#define MAX_BITS 32

#define MINUS_BIT LAST_BIT
#define NAN_SCALE 0x407F8000
#define INF_SCALE 255
#define MAX_SCALE 28
#define SCALE_BIT 16
#define SCALE_LEN 8

#define FUNC_ELEM 3
#define HIGHEST_DIGIT 2
#define MIDDLE_DIGIT 1
#define LOWEST_DIGIT 0

#define HIGHEST_BIG_DIGIT 6
#define LOWEST_BIG_DIGIT 0

#define BIG_DIGIT ((s21_big_decimal){{10U, 0U, 0U, 0U, 0U, 0U}})
#define BIG_UNIT ((s21_big_decimal){{1U, 0U, 0U, 0U, 0U, 0U}})
#define DEC_DIGIT ((s21_decimal){{10U, 0U, 0U, 0U}})
#define DEC_UNIT ((s21_decimal){{1U, 0U, 0U, 0U}})

#define BIG_BY_INT(num) ((s21_big_decimal){{(num)}})
#define TO_BIG(dec) \
  ((s21_big_decimal){{(dec).bits[0], (dec).bits[1], (dec).bits[2], 0U, 0U, 0U}})
#define TO_NORMAL(big_dec) \
  ((s21_decimal){{(big_dec).bits[0], (big_dec).bits[1], (big_dec).bits[2], 0U}})

#define DEL_MANTISSA(dec) \
  do {                    \
    (dec).bits[0] = 0U;   \
    (dec).bits[1] = 0U;   \
    (dec).bits[2] = 0U;   \
  } while (0)

#define SET_NAN(dec)            \
  do {                          \
    (dec).bits[0] = UINT32_MAX; \
    (dec).bits[1] = UINT32_MAX; \
    (dec).bits[2] = UINT32_MAX; \
    (dec).bits[3] = NAN_SCALE;  \
  } while (0)

/*
  BASE MACROSES
*/

#define S21_IS_NEGATIVE(dec) (get_bit((dec).bits[FUNC_ELEM], MINUS_BIT))
#define S21_IS_INF(dec) \
  ((get_scale((dec)) == INF_SCALE) && !IS_MANTISSA_TRUE(dec))
#define S21_IS_NAN(dec) \
  ((get_scale((dec)) == INF_SCALE) && IS_MANTISSA_TRUE(dec))
#define S21_IS_TRUE(dec) (get_scale((dec)) || IS_MANTISSA_TRUE(dec))
#define IS_MANTISSA_TRUE(dec)                                       \
  ({                                                                \
    uint8_t __result = 0;                                           \
    for (uint8_t __i = LOWEST_DIGIT; __i <= HIGHEST_DIGIT; __i++) { \
      if ((dec).bits[__i]) {                                        \
        __result = 1;                                               \
        break;                                                      \
      }                                                             \
    }                                                               \
    __result;                                                       \
  })
#define IS_BIG_MANTISSA_TRUE(big_dec)                                       \
  ({                                                                        \
    uint8_t __result = 0;                                                   \
    for (uint8_t __i = LOWEST_BIG_DIGIT; __i <= HIGHEST_BIG_DIGIT; __i++) { \
      if ((big_dec).bits[__i]) {                                            \
        __result = 1;                                                       \
        break;                                                              \
      }                                                                     \
    }                                                                       \
    __result;                                                               \
  })
#define IS_OVERFLOW(big_dec)                                            \
  ({                                                                    \
    uint8_t __result = 0;                                               \
    for (uint8_t __i = HIGHEST_BIG_DIGIT; __i > HIGHEST_DIGIT; __i--) { \
      if ((big_dec).bits[__i]) {                                        \
        __result = 1;                                                   \
        break;                                                          \
      }                                                                 \
    }                                                                   \
    __result;                                                           \
  })

/*
  BIT CALLS
*/

uint8_t get_bit(uint32_t val, uint8_t shift);
void swap_bit(uint32_t *val, uint8_t shift);
void set_bit(uint32_t *val, uint8_t shift);
void del_bit(uint32_t *val, uint8_t shift);

/*
  BITWISE OPERATORS
*/

s21_big_decimal big_dec_left(s21_big_decimal dec, uint8_t shift);
s21_big_decimal big_dec_right(s21_big_decimal dec, uint8_t shift);
s21_big_decimal big_dec_not(s21_big_decimal res);
void big_dec_and(s21_big_decimal dec1, s21_big_decimal dec2,
                 s21_big_decimal *res);
void big_dec_xor(s21_big_decimal dec1, s21_big_decimal dec2,
                 s21_big_decimal *res);

/*
  SCALE OPERATORS
*/

uint8_t get_scale(s21_decimal dec);
void del_scale(s21_decimal *dec);
void set_scale(s21_decimal *dec, uint32_t scale);
void set_sign(s21_decimal *dec, uint8_t sign);

/*
  OTHER COMMON OPERATORS
*/

s21_decimal dec_abs(s21_decimal dec);
void shift_denominators(s21_decimal *lower, s21_decimal *higher);
void bank_round(s21_big_decimal *dec, uint8_t remainder);
uint8_t get_highest_dec_digit(s21_big_decimal dec);

/*
  ARITHMETIC OPERATORS
*/

int s21_add(s21_decimal dec1, s21_decimal dec2, s21_decimal *res);
int s21_sub(s21_decimal dec1, s21_decimal dec2, s21_decimal *res);
int s21_mul(s21_decimal dec1, s21_decimal dec2, s21_decimal *res);
int s21_div(s21_decimal dec1, s21_decimal dec2, s21_decimal *res);
uint8_t addition(s21_big_decimal dec1, s21_big_decimal dec2,
                 s21_big_decimal *res);
void subtraction(s21_big_decimal dec1, s21_big_decimal dec2,
                 s21_big_decimal *res);
uint8_t multiplication(s21_big_decimal dec1, s21_big_decimal dec2,
                       s21_big_decimal *res);
s21_big_decimal division_int(s21_big_decimal dec1, s21_big_decimal dec2,
                             s21_big_decimal *res);
void division_fract(s21_big_decimal rem, s21_big_decimal dec,
                    s21_big_decimal *res, int16_t *scale);

/*
  COMPARISON OPERATORS
*/

int s21_is_less(s21_decimal dec1, s21_decimal dec2);
int s21_is_less_or_equal(s21_decimal dec1, s21_decimal dec2);
int s21_is_greater(s21_decimal dec1, s21_decimal dec2);
int s21_is_greater_or_equal(s21_decimal dec1, s21_decimal dec2);
int s21_is_equal(s21_decimal dec1, s21_decimal dec2);
int s21_is_not_equal(s21_decimal dec1, s21_decimal dec2);

int comparison(s21_decimal dec1, s21_decimal dec2);
int who_is_bigger(int num1, int num2);
int bits_cmp(s21_decimal dec1, s21_decimal dec2);

/*
  CONVERT FUNCTIONS
*/
int s21_from_int_to_decimal(int src, s21_decimal *dst);
int s21_from_float_to_decimal(float src, s21_decimal *dst);
int s21_from_decimal_to_int(s21_decimal src, int *dst);
int s21_from_decimal_to_float(s21_decimal src, float *dst);

int s21_from_decimal_to_double(s21_decimal src, double *dst);
double convert_mantissa_to_double(s21_decimal bits_array);

/*
  ANOTHER FUNCTIONS
*/

int s21_floor(s21_decimal dec, s21_decimal *res);
int s21_round(s21_decimal dec, s21_decimal *res);
int s21_truncate(s21_decimal dec, s21_decimal *res);
int s21_negate(s21_decimal dec, s21_decimal *res);

void fill_mantissa(s21_decimal *res, double tmp_double);
int get_not_use_bits(s21_decimal bits_array);

#endif
