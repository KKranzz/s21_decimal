#include "../s21_decimal.h"

#include "../s21_utils.h"

s21_decimal s21_string_to_decimal(const char* decimal_format) {
  s21_decimal init_dec = s21_decimal_init();

  if (decimal_format != NULL && s21_number_is_correct(decimal_format) == 1) {
    if (decimal_format[0] == '-') {
      s21_negate(init_dec, &init_dec);
    }
    char num_str[32] = {0};
    s21_parse_dot(decimal_format, &init_dec);
    s21_remove_sign_and_dot(decimal_format, num_str);
    s21_str_to_bits(num_str, init_dec.bits);
  }

  return init_dec;
}

s21_decimal s21_decimal_init() {
  s21_decimal empty_dec = {0};
  return empty_dec;
}

int s21_negate(s21_decimal value, s21_decimal* result) {
  int code_func = 0;
  if (result != NULL) {
    value.bits[3] ^= (1 << 31);
    *result = value;
  } else {
    code_func = 1;
  }

  return code_func;
}

int s21_number_is_correct(const char* decimal_format) {
  int is_correct = 1;
  size_t len = strlen(decimal_format);

  if (!(len == 0 || len >= (size_t)(31 + (decimal_format[0] == '+' ||
                                          decimal_format[0] == '-')))) {
    int sign = 0;
    int dot_pos = -1;
    int digit_count = 0;

    if (decimal_format[0] == '+' || decimal_format[0] == '-') {
      sign = 1;
    } else {
      sign = 0;
    }

    for (int i = sign; i < (int)len; i++) {
      if (decimal_format[i] == '.') {
        if (dot_pos != -1) {
          i = len;
          is_correct = 0;
        }
        dot_pos = i;
        if ((int)i == sign || (int)i == (int)len - 1) {
          i = len;
          is_correct = 0;
        }
      } else if (!isdigit(decimal_format[i])) {
        i = len;
        is_correct = 0;
      } else {
        digit_count++;
      }
    }
    if (!(digit_count < 1 || digit_count > 29) && is_correct != 0) {
      is_correct = 1;
    }
  } else {
    is_correct = 0;
  }

  return is_correct;
}

void s21_parse_dot(const char* str, s21_decimal* decimal) {
  const char* decimal_point = strchr(str, '.');
  if (decimal_point != NULL) {
    int fractional_length = strlen(decimal_point + 1);
    decimal->bits[3] |= (fractional_length << 16);
  }
}

void s21_remove_sign_and_dot(const char* src, char* dest) {
  size_t j = 0;
  for (size_t i = 0; src[i]; i++) {
    if (src[i] != '+' && src[i] != '-' && src[i] != '.') {
      dest[j++] = src[i];
    }
  }
  dest[j] = '\0';
}

void s21_str_to_bits(const char* num_str, int bits[3]) {
  bits[0] = bits[1] = bits[2] = 0;
  char current[100] = {0};
  strcpy(current, num_str);

  for (int power = 95; power >= 0; power--) {
    const char* power_str = s21_get_power_of_two(power);
    while (s21_compare_str(current, power_str) >= 0) {
      char temp[100] = {0};
      s21_subtract_str(current, power_str, temp);
      strcpy(current, temp);
      s21_set_bit_arr(bits, power);
    }
  }

  if (strcmp(current, "0") != 0) {
    memset(bits, 0, 3 * sizeof(int));
  }
}

void s21_subtract_str(const char* a, const char* b, char* result) {
  char a_copy[100] = {0}, b_copy[100] = {0};
  strcpy(a_copy, a);
  strcpy(b_copy, b);

  int len_a = strlen(a_copy);
  int len_b = strlen(b_copy);
  int max_len = len_a > len_b ? len_a : len_b;

  s21_reverse_string(a_copy);
  s21_reverse_string(b_copy);

  int borrow = 0;
  int i;
  for (i = 0; i < max_len; i++) {
    int digit_a = (i < len_a) ? (a_copy[i] - '0') : 0;
    int digit_b = (i < len_b) ? (b_copy[i] - '0') : 0;

    digit_a -= borrow;
    if (digit_a < digit_b) {
      digit_a += 10;
      borrow = 1;
    } else {
      borrow = 0;
    }

    result[i] = (digit_a - digit_b) + '0';
  }

  while (i > 1 && result[i - 1] == '0') i--;
  result[i] = '\0';
  s21_reverse_string(result);
}

int s21_compare_str(const char* a, const char* b) {
  int result = 0;
  while (*a == '0' && *(a + 1)) a++;
  while (*b == '0' && *(b + 1)) b++;

  size_t len_a = strlen(a);
  size_t len_b = strlen(b);
  if (len_a != len_b) {
    result = (len_a > len_b) ? 1 : -1;
  } else {
    result = strcmp(a, b);
  }
  return result;
}

void s21_reverse_string(char* str) {
  int len = strlen(str);
  for (int i = 0; i < len / 2; i++) {
    char temp = str[i];
    str[i] = str[len - i - 1];
    str[len - i - 1] = temp;
  }
}

void s21_set_bit_arr(int bits[], int power) {
  int index = 0 + power / 32;
  int bit_pos = power % 32;
  bits[index] |= (1u << bit_pos);
}

void s21_decimal_to_string(s21_decimal dec, char* result) {
  s21_bits_to_str(dec.bits, result);
}

void s21_bits_to_str(const int bits[4], char* result) {
  strcpy(result, "0");
  char temp[100] = {0};
  int is_negative = bits[3] >> 31;
  int fractional_count = (bits[3] >> 16) & 0xFF;

  for (int power = 0; power <= 95; power++) {
    int index = power / 32, bit_pos = power % 32;
    if (bits[index] & (1u << bit_pos)) {
      const char* power_str = s21_get_power_of_two(power);
      s21_add_str(result, power_str, temp);
      strcpy(result, temp);
    }
  }

  if (fractional_count > 0) {
    int len = strlen(result);
    char final[100] = {0};
    if (len <= fractional_count) {
      int zeros = fractional_count - len;
      char zeros_str[30] = {0};
      memset(zeros_str, '0', zeros);
      snprintf(final, sizeof(final), "0.%s%s", zeros_str, result);
    } else {
      snprintf(final, sizeof(final), "%.*s.%.*s", len - fractional_count,
               result, fractional_count, result + (len - fractional_count));
    }
    strcpy(result, final);
    s21_remove_trailing_zeros(result);
  }

  if (is_negative) {
    char temp_res[100] = {0};
    snprintf(temp_res, sizeof(temp_res), "-%s", result);
    strcpy(result, temp_res);
  }
}

void s21_add_str(const char* a, const char* b, char* result) {
  char a_copy[100] = {0}, b_copy[100] = {0};
  strcpy(a_copy, a);
  strcpy(b_copy, b);

  int len_a = strlen(a_copy);
  int len_b = strlen(b_copy);
  int max_len = (len_a > len_b) ? len_a : len_b;

  s21_reverse_string(a_copy);
  s21_reverse_string(b_copy);

  int carry = 0;
  int i;
  for (i = 0; i < max_len || carry; i++) {
    int digit_a = (i < len_a) ? (a_copy[i] - '0') : 0;
    int digit_b = (i < len_b) ? (b_copy[i] - '0') : 0;

    int sum = digit_a + digit_b + carry;
    carry = sum / 10;
    result[i] = (sum % 10) + '0';
  }

  result[i] = '\0';
  s21_reverse_string(result);
}

void s21_show_bits(s21_decimal dig) {
  printf("Bits: [%08x, %08x, %08x, %08x]\n", dig.bits[3], dig.bits[2],
         dig.bits[1], dig.bits[0]);
}

//
int s21_is_overflow(s21_big_decimal num) {
  return num.decs[0].bits[3] != 0 || num.decs[1].bits[0] != 0 ||
         num.decs[1].bits[1] != 0 || num.decs[1].bits[2] != 0 ||
         num.decs[1].bits[3] != 0;
}

void s21_bank_round(s21_big_decimal* num, unsigned remainder, int has_rem) {
  if (!(remainder < 5)) {
    s21_big_decimal temp = *num;
    unsigned last_digit = 0;
    s21_div_by_10_with_remainder(&temp, &last_digit);

    if (remainder > 5 || (remainder == 5 && (last_digit % 2 != 0)) ||
        (remainder == 5 && has_rem == 1)) {
      s21_big_decimal one = s21_convert_to_big(s21_string_to_decimal("1"));
      s21_add_mantissa(*num, one, num);
    }
  }
}

void s21_div_by_10_with_remainder(s21_big_decimal* num, unsigned* rem) {
  unsigned long long remainder = 0;
  for (int i = 7; i >= 0; i--) {
    int part = i / 4, idx = i % 4;
    unsigned long long value =
        (remainder << 32) | (unsigned)num->decs[part].bits[idx];
    num->decs[part].bits[idx] = (int)(value / 10);
    remainder = value % 10;
  }
  *rem = (unsigned)remainder;
}

int s21_normalize_big_decimal(s21_big_decimal* num) {
  int exit = 0;
  while (num->dot_data > 0 && exit == 0) {
    s21_big_decimal temp = *num;
    unsigned rem = 0;
    s21_div_by_10_with_remainder(&temp, &rem);
    if (rem != 0) {
      exit = 1;
    } else {
      *num = temp;
      num->dot_data--;
    }
  }
  int has_rem = 0;
  while (num->dot_data > 28) {
    unsigned rem = 0;
    s21_div_by_10_with_remainder(num, &rem);
    num->dot_data--;
    if (!(num->dot_data > 28)) {
      s21_bank_round(num, rem, has_rem);
    } else {
      if (rem != 0) {
        has_rem = 1;
      }
    }
  }
  has_rem = 0;
  while (s21_is_overflow(*num) && num->dot_data > 0) {
    unsigned rem = 0;
    s21_div_by_10_with_remainder(num, &rem);
    num->dot_data--;
    if (!(s21_is_overflow(*num) && num->dot_data > 0)) {
      s21_bank_round(num, rem, has_rem);
    } else {
      if (rem != 0) {
        has_rem = 1;
      }
    }
  }

  return s21_is_overflow(*num) ? 1 : 0;
}

int s21_extract_scale(s21_decimal d) {
  return ((unsigned)d.bits[3] >> 16) & 0xFF;
}

int s21_extract_sign(s21_decimal d) { return (d.bits[3] >> 31) & 0x01; }

void s21_set_scale_sign(s21_decimal* d, int scale, int sign) {
  d->bits[3] = (scale << 16) | (sign << 31);
}

s21_big_decimal s21_convert_to_big(s21_decimal d) {
  s21_big_decimal big;
  big.decs[0] = d;
  big.decs[1] = s21_decimal_init();
  big.dot_data = s21_extract_scale(d);
  big.sign = s21_extract_sign(d);
  big.decs[0].bits[3] = 0;
  return big;
}

int s21_convert_to_decimal(s21_big_decimal big, s21_decimal* result) {
  int code_func = 0;
  if (s21_normalize_big_decimal(&big)) {
    code_func = big.sign == 1 ? 2 : 1;
  }
  if (code_func == 0) {
    *result = big.decs[0];
    s21_set_scale_sign(result, big.dot_data, big.sign);
  }

  return code_func;
}

int s21_multiply_by_10(s21_big_decimal* num) {
  unsigned carry = 0;
  for (int i = 0; i < 8; i++) {
    int part = i / 4, idx = i % 4;

    unsigned val = (unsigned)num->decs[part].bits[idx];
    unsigned long long temp =
        (unsigned long long)val * 10ULL + (unsigned long long)carry;
    num->decs[part].bits[idx] = (int)(temp & 0xFFFFFFFF);
    carry = (unsigned)(temp >> 32);
  }
  return carry ? 1 : 0;
}

int s21_multiply_by_pow10(s21_big_decimal* num, int power) {
  int code_func = 0;
  if (power < 0 || power > 28) {
    code_func = 1;
  }
  for (int i = 0; i < power && code_func == 0; i++)
    if (s21_multiply_by_10(num)) {
      code_func = 1;
    }

  return code_func;
}

int s21_correct_pow(s21_big_decimal* a, s21_big_decimal* b) {
  int diff = a->dot_data - b->dot_data;
  int code_func = 0;
  if (diff != 0) {
    s21_big_decimal* target = diff > 0 ? b : a;
    int power = abs(diff);

    if (target->dot_data + power > 28) {
      code_func = 1;
    }

    s21_multiply_by_pow10(target, power);

    if (code_func != 1) {
      target->dot_data += power;
    }
  }
  return code_func;
}

int s21_compare_mantissa(s21_big_decimal a, s21_big_decimal b) {
  int res = 0;
  for (int i = 7; i >= 0; i--) {
    int part = i / 4, idx = i % 4;
    unsigned va = (unsigned)a.decs[part].bits[idx];
    unsigned vb = (unsigned)b.decs[part].bits[idx];
    if (va != vb) {
      res = va > vb ? 1 : -1;
      i = -1;
    }
  }
  return res;
}

int s21_add_mantissa(s21_big_decimal a, s21_big_decimal b,
                     s21_big_decimal* res) {
  unsigned carry = 0;

  for (int i = 0; i < 8; i++) {
    int part = i / 4, idx = i % 4;

    unsigned a_bits = (unsigned)a.decs[part].bits[idx];
    unsigned b_bits = (unsigned)b.decs[part].bits[idx];
    unsigned long long sum =
        (unsigned long long)a_bits + (unsigned long long)b_bits + carry;
    res->decs[part].bits[idx] = (int)(sum & 0xFFFFFFFF);
    carry = (unsigned)(sum >> 32);
  }

  int code_func = 0;
  if (carry != 0) {
    code_func = res->sign ? 2 : 1;
  }
  return code_func;
}

int s21_sub_mantissa(s21_big_decimal a, s21_big_decimal b,
                     s21_big_decimal* res) {
  unsigned borrow = 0;
  int code_func = 0;
  for (int i = 0; i < 8; i++) {
    int part = i / 4, idx = i % 4;

    unsigned a_bits = (unsigned)a.decs[part].bits[idx];
    unsigned b_bits = (unsigned)b.decs[part].bits[idx];
    unsigned long long a_val = (unsigned long long)a_bits;
    unsigned long long b_val =
        (unsigned long long)b_bits + (unsigned long long)borrow;

    if (a_val < b_val) {
      a_val += 0x100000000ULL;
      borrow = 1;
    } else {
      borrow = 0;
    }

    res->decs[part].bits[idx] = (int)(a_val - b_val);
  }

  if (borrow != 0) {
    code_func = 2;
  }
  return code_func;
}

int s21_add(s21_decimal v1, s21_decimal v2, s21_decimal* result) {
  int code_func = 0;
  int is_equal = 0;
  s21_big_decimal a = s21_convert_to_big(v1);
  s21_big_decimal b = s21_convert_to_big(v2);
  s21_big_decimal res = {0};

  s21_correct_pow(&a, &b);

  if (a.sign == b.sign) {
    res.sign = a.sign;
    code_func = s21_add_mantissa(a, b, &res);

  } else if (code_func == 0) {
    int cmp = s21_compare_mantissa(a, b);
    if (cmp == 0) {
      *result = (s21_decimal){0};
      is_equal = 1;
    } else {
      code_func =
          cmp > 0 ? s21_sub_mantissa(a, b, &res) : s21_sub_mantissa(b, a, &res);
      res.sign = cmp > 0 ? a.sign : b.sign;
    }
  }
  if (is_equal != 1 && code_func == 0) {
    res.dot_data = a.dot_data;
    code_func = s21_convert_to_decimal(res, result);
  }

  if (code_func != 0) {
    *result = (s21_decimal){0};
  }
  return code_func;
}

int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal* result) {
  int code_func = 0;
  int is_equal = 0;
  s21_big_decimal a = s21_convert_to_big(value_1);
  s21_big_decimal b = s21_convert_to_big(value_2);
  s21_big_decimal res = {0};

  s21_correct_pow(&a, &b);

  if (a.sign == b.sign) {
    int cmp = s21_compare_mantissa(a, b);
    if (cmp == 0) {
      *result = (s21_decimal){0};
      is_equal = 1;
    } else if (cmp > 0) {
      code_func = s21_sub_mantissa(a, b, &res);
      res.sign = a.sign;
    } else {
      code_func = s21_sub_mantissa(b, a, &res);
      res.sign = !b.sign;
    }
  } else {
    code_func = s21_add_mantissa(a, b, &res);
    res.sign = a.sign;
  }

  if (code_func == 0 && is_equal == 0) {
    res.dot_data = a.dot_data;
    code_func = s21_convert_to_decimal(res, result);
  }
  if (code_func != 0 || is_equal == 1) {
    *result = (s21_decimal){0};
  }
  return code_func;
}

int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal* result) {
  int code_func = 0;
  s21_big_decimal a = s21_convert_to_big(value_1);
  s21_big_decimal b = s21_convert_to_big(value_2);
  s21_big_decimal res = {0};

  for (int i = 0; i < 256; i++) {
    if (s21_get_bit(a, i)) {
      s21_big_decimal temp = b;
      s21_shift_left(&temp, i);
      s21_add_mantissa(res, temp, &res);
    }
  }

  res.sign = a.sign ^ b.sign;
  res.dot_data = a.dot_data + b.dot_data;
  code_func = s21_convert_to_decimal(res, result);

  return code_func;
}

int s21_div(s21_decimal v1, s21_decimal v2, s21_decimal* result) {
  int code = 0;
  if (!s21_is_zero(v2)) {
    s21_big_decimal dividend = s21_convert_to_big(v1),
                    divisor = s21_convert_to_big(v2), quotient = {0},
                    remainder = {0};

    s21_correct_pow(&dividend, &divisor);

    for (int i = 0; i < 256 && !code; i++) {
      s21_shift_left(&remainder, 1);
      remainder.decs[0].bits[0] |= s21_get_bit(dividend, 255 - i);

      if (s21_compare_mantissa(remainder, divisor) >= 0) {
        code = s21_sub_mantissa(remainder, divisor, &remainder);
        s21_set_bit(&quotient, 255 - i);
      }
    }

    int curr_scale = 0;
    while (!code && curr_scale < 28 && !s21_is_zero_big(remainder)) {
      s21_multiply_by_10(&remainder);
      unsigned d = 0;
      while (s21_compare_mantissa(remainder, divisor) >= 0) {
        d++, code = s21_sub_mantissa(remainder, divisor, &remainder);
      }
      s21_multiply_by_10(&quotient);
      quotient.decs[0].bits[0] += d;
      curr_scale++;
    }

    if (code == 0) {
      quotient.dot_data = curr_scale;
      quotient.sign = dividend.sign ^ divisor.sign;
      code = s21_convert_to_decimal(quotient, result);
    }
  } else {
    code = 3;
  }

  return code;
}

int s21_is_zero(s21_decimal d) {
  return d.bits[0] == 0 && d.bits[1] == 0 && d.bits[2] == 0;
}

void s21_shift_left(s21_big_decimal* num, int n) {
  for (int i = 0; i < n; i++) {
    unsigned carry = 0;
    for (int j = 0; j < 8; j++) {
      int dec_index = j / 4, bit_index = j % 4;

      unsigned val = (unsigned)num->decs[dec_index].bits[bit_index];
      unsigned new_val = (val << 1) | carry;
      carry = (val >> 31) & 1;
      num->decs[dec_index].bits[bit_index] = (int)new_val;
    }
  }
}

int s21_get_bit(s21_big_decimal num, int pos) {
  int res = 0;
  if (pos < 0 || pos >= 256) {
    res = 0;
  } else {
    int dec_index = pos / 128;
    int bit_in_dec = pos % 128;
    int part = bit_in_dec / 32;
    int idx = bit_in_dec % 32;
    unsigned val = (unsigned)num.decs[dec_index].bits[part];
    res = (val >> idx) & 1;
  }
  return res;
}

void s21_set_bit(s21_big_decimal* num, int pos) {
  if (pos < 0 || pos >= 256) {
  } else {
    int dec_index = pos / 128;
    int bit_in_dec = pos % 128;
    int part = bit_in_dec / 32;
    int idx = bit_in_dec % 32;
    num->decs[dec_index].bits[part] |= (1U << idx);
  }
}

void s21_show_decimal(s21_decimal dig) {
  char str[32] = {0};
  s21_decimal_to_string(dig, str);
  printf("%s\n", str);
}

void s21_show_big_decimal(s21_big_decimal dig) {
  char str[32] = {0};
  s21_decimal res = s21_decimal_init();
  s21_convert_to_decimal(dig, &res);
  s21_decimal_to_string(res, str);
  printf("%s\n", str);
}

int s21_is_zero_big(s21_big_decimal num) {
  int result = 1;
  for (int i = 0; i < 4; i++) {
    if (num.decs[0].bits[i] != 0 || num.decs[1].bits[i] != 0) {
      result = 0;
      i = 4;
    }
  }
  return result;
}

int s21_floor(s21_decimal value, s21_decimal* result) {
  int code = 0;
  if (!result) {
    code = 1;
  } else {
    s21_big_decimal big = s21_convert_to_big(value);
    s21_big_decimal one = s21_convert_to_big(s21_string_to_decimal("1"));

    unsigned rem = 0;

    while (big.dot_data > 0) {
      s21_div_by_10_with_remainder(&big, &rem);
      if (rem != 0 && big.sign) {
        s21_add_mantissa(big, one, &big);
      }
      big.dot_data--;
      rem = 0;
    }

    s21_normalize_big_decimal(&big);

    code = s21_convert_to_decimal(big, result);
  }
  return code;
}

int s21_round(s21_decimal value, s21_decimal* result) {
  int code = 0;
  if (!result) {
    code = 1;
  } else {
    s21_big_decimal big = s21_convert_to_big(value);
    int original_scale = big.dot_data;
    unsigned long long total_remainder = 0;
    unsigned long long multiplier = 1;

    for (int i = 0; i < original_scale; i++) {
      unsigned current_rem = 0;
      s21_div_by_10_with_remainder(&big, &current_rem);
      total_remainder += current_rem * multiplier;
      multiplier *= 10;
      big.dot_data--;
    }

    unsigned long long threshold = 5 * (multiplier / 10);
    if (total_remainder >= threshold) {
      s21_big_decimal one = s21_convert_to_big(s21_string_to_decimal("1"));
      s21_add_mantissa(big, one, &big);
    }

    s21_normalize_big_decimal(&big);

    code = s21_convert_to_decimal(big, result);
  }

  return code;
}

int s21_truncate(s21_decimal value, s21_decimal* result) {
  int code = 0;
  if (result == NULL) {
    code = 1;
  } else {
    s21_big_decimal big = s21_convert_to_big(value);
    int scale = big.dot_data;

    for (int i = 0; i < scale; i++) {
      unsigned rem = 0;
      s21_div_by_10_with_remainder(&big, &rem);
    }
    big.dot_data = 0;

    s21_convert_to_decimal(big, result);
  }
  code = code == 2 || code == 1 ? 1 : 0;

  return code;
}

int s21_is_less(s21_decimal a, s21_decimal b) {
  int result = 0;

  if (!(s21_is_zero(a) && s21_is_zero(b))) {
    int sign_a = s21_extract_sign(a);
    int sign_b = s21_extract_sign(b);

    if (sign_a != sign_b) {
      result = sign_a == 1;

    } else {
      s21_big_decimal big_a = s21_convert_to_big(a);
      s21_big_decimal big_b = s21_convert_to_big(b);
      s21_correct_pow(&big_a, &big_b);

      const int is_abs_less = (s21_compare_mantissa(big_a, big_b) == -1);
      result = is_abs_less;

      if (sign_a == 1) {
        result = !result;
      }
    }
  }

  return result;
}

int s21_is_equal(s21_decimal a, s21_decimal b) {
  int result = 1;

  if (!(s21_is_zero(a) && s21_is_zero(b))) {
    int sign_a = s21_extract_sign(a);
    int sign_b = s21_extract_sign(b);

    if (sign_a != sign_b) {
      result = 0;
    } else {
      s21_big_decimal big_a = s21_convert_to_big(a);
      s21_big_decimal big_b = s21_convert_to_big(b);
      s21_correct_pow(&big_a, &big_b);

      const int cmp = s21_compare_mantissa(big_a, big_b);
      const int is_equal = cmp == 0;

      result = is_equal;
    }
  }

  return result;
}

int s21_is_less_or_equal(s21_decimal a, s21_decimal b) {
  return s21_is_less(a, b) || s21_is_equal(a, b);
}

int s21_is_greater(s21_decimal a, s21_decimal b) {
  return !s21_is_less_or_equal(a, b);
}

int s21_is_greater_or_equal(s21_decimal a, s21_decimal b) {
  return s21_is_greater(a, b) || s21_is_equal(a, b);
}

int s21_is_not_equal(s21_decimal a, s21_decimal b) {
  return !s21_is_equal(a, b);
}

int s21_from_int_to_decimal(int src, s21_decimal* dst) {
  int code = 0;
  if (!dst) {
    code = 1;
  } else {
    *dst = s21_decimal_init();

    if (src < 0) {
      dst->bits[3] |= 1u << 31;
      src = -src;
    }

    dst->bits[0] = src;
  }
  return code;
}

int s21_from_decimal_to_int(s21_decimal src, int* dst) {
  int code = 0;
  if (dst == NULL) {
    code = 1;
  } else {
    s21_decimal truncated = s21_decimal_init();
    s21_truncate(src, &truncated);

    int sign = (truncated.bits[3] >> 31) & 1;
    int scale = (truncated.bits[3] >> 16) & 0xFF;
    if (scale != 0 || truncated.bits[1] != 0 || truncated.bits[2] != 0) {
      code = 1;
    } else {
      unsigned int mantissa = truncated.bits[0];
      if ((sign && mantissa > (unsigned int)INT_MAX + 1) ||
          (!sign && mantissa > INT_MAX)) {
        code = 1;
      } else {
        *dst = sign ? -(int)mantissa : (int)mantissa;
      }
    }
  }
  return code;
}

int s21_from_float_to_decimal(float src, s21_decimal* dst) {
  int code = 1;
  if (dst) {
    *dst = s21_decimal_init();
    int sign = src < 0;
    float abs_src = sign ? -src : src;
    int is_nan_or_inf = isnan(abs_src) || isinf(abs_src);
    int is_zero = (abs_src == 0.0f);
    int valid_range =
        (abs_src >= 1e-28f && abs_src <= 7.9228162514264337593e+28f);

    if (!is_nan_or_inf && !is_zero && valid_range) {
      double d = (double)abs_src;
      char buffer[32] = {0};
      snprintf(buffer, sizeof(buffer), "%.7g", d);

      if (!strchr(buffer, 'e') && !strchr(buffer, 'E')) {
        s21_remove_trailing_zeros(buffer);
        *dst = s21_string_to_decimal(buffer);
        if (sign) s21_negate(*dst, dst);
        code = s21_is_zero(*dst) && !is_zero ? 1 : 0;
      }
    } else if (is_zero) {
      code = 0;
    }
  }
  return code;
}

int s21_from_decimal_to_float(s21_decimal src, float* dst) {
  int code = 0;
  double d = 0.0;
  char buffer[512] = {0}, rounded[32] = {0};

  if (!dst) {
    code = 1;
  } else {
    s21_decimal_to_string(src, buffer);
    sscanf(buffer, "%lf", &d);
    int is_tiny = fabs(d) < 1e-28;

    if (is_tiny) {
      *dst = 0.0f;
      code = (d != 0.0) ? 1 : 0;
    } else {
      snprintf(rounded, sizeof(rounded), "%.7g", d);
      code = (sscanf(rounded, "%f", dst) != 1);
    }
  }

  return code;
}

void s21_remove_trailing_zeros(char* buffer) {
  char* dot = strchr(buffer, '.');
  if (dot) {
    char* end = buffer + strlen(buffer) - 1;
    while (end > dot && *end == '0') {
      *end-- = '\0';
    }
    if (end == dot) {
      *dot = '\0';
    }
  }
}