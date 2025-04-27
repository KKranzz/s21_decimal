#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef S21_DECIMAL_H
#define S21_DECIMAL_H

typedef struct {
  int bits[4];
} s21_decimal;

typedef struct {
  s21_decimal decs[2];
  int dot_data;
  int sign;
} s21_big_decimal;

s21_decimal s21_decimal_init();

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal* result);

int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal* result);

int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal* result);

int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal* result);

int s21_is_less(s21_decimal a, s21_decimal b);

int s21_is_less_or_equal(s21_decimal, s21_decimal);

int s21_is_greater(s21_decimal a, s21_decimal b);

int s21_is_greater_or_equal(s21_decimal a, s21_decimal b);

int s21_is_equal(s21_decimal a, s21_decimal b);

int s21_is_not_equal(s21_decimal a, s21_decimal b);

int s21_from_int_to_decimal(int src, s21_decimal* dst);

int s21_from_float_to_decimal(float src, s21_decimal* dst);

int s21_from_decimal_to_float(s21_decimal src, float* dst);

int s21_from_decimal_to_int(s21_decimal src, int* dst);

int s21_floor(s21_decimal value, s21_decimal* result);

int s21_round(s21_decimal value, s21_decimal* result);

int s21_truncate(s21_decimal value, s21_decimal* result);

int s21_negate(s21_decimal value, s21_decimal* result);

int s21_add_mantissa(s21_big_decimal a, s21_big_decimal b,
                     s21_big_decimal* res);

int s21_is_overflow(s21_big_decimal num);

void s21_bank_round(s21_big_decimal* num, unsigned remainder, int has_rem);

void s21_div_by_10_with_remainder(s21_big_decimal* num, unsigned* rem);

int s21_normalize_big_decimal(s21_big_decimal* num);

int s21_extract_scale(s21_decimal d);

int s21_extract_sign(s21_decimal d);

void s21_set_scale_sign(s21_decimal* d, int scale, int sign);

s21_big_decimal s21_convert_to_big(s21_decimal d);

int s21_convert_to_decimal(s21_big_decimal big, s21_decimal* result);

int s21_multiply_by_10(s21_big_decimal* num);

int s21_multiply_by_pow10(s21_big_decimal* num, int power);

int s21_correct_pow(s21_big_decimal* a, s21_big_decimal* b);

int s21_compare_mantissa(s21_big_decimal a, s21_big_decimal b);

int s21_sub_mantissa(s21_big_decimal a, s21_big_decimal b,
                     s21_big_decimal* res);

int s21_is_zero(s21_decimal d);

void s21_set_bit(s21_big_decimal* num, int pos);

void s21_shift_left(s21_big_decimal* num, int n);

int s21_get_bit(s21_big_decimal num, int pos);

s21_decimal s21_string_to_decimal(const char* decimal_format);

int s21_number_is_correct(const char* decimal_format);

void s21_parse_dot(const char* str, s21_decimal* decimal);

void s21_remove_sign_and_dot(const char* src, char* dest);

void s21_str_to_bits(const char* num_str, int bits[3]);

void s21_subtract_str(const char* a, const char* b, char* result);

int s21_compare_str(const char* a, const char* b);

void s21_reverse_string(char* str);

void s21_set_bit_arr(int bits[], int power);

void s21_decimal_to_string(s21_decimal dec, char* result);

void s21_bits_to_str(const int bits[4], char* result);

void s21_add_str(const char* a, const char* b, char* result);

void s21_show_bits(s21_decimal dig);

void s21_show_decimal(s21_decimal dig);

void s21_show_big_decimal(s21_big_decimal);

int s21_is_zero_big(s21_big_decimal num);

void s21_remove_trailing_zeros(char* buffer);

#endif  // !S21_DECIMAL_H