#include <stdio.h>

void double_to_string(double value, char *str) {
  int integer_part = (int)value;  // Extract the integer part
  double fractional_part = value - integer_part;  // Extract the fractional part

  // Convert the integer part to string
  int i = 0;
  if (integer_part == 0) {
    str[i++] = '0';  // Special case for 0
  } else {
    int temp = integer_part;
    int digits[20];  // Array to store digits in reverse
    int digit_count = 0;

    // Extract digits in reverse order
    while (temp > 0) {
      digits[digit_count++] = temp % 10;
      temp /= 10;
    }

    // Place digits into the string in correct order
    for (int j = digit_count - 1; j >= 0; j--) {
      str[i++] = digits[j] + '0';
    }
  }

  // Add the decimal point
  str[i++] = '.';

  // Convert fractional part to string
  for (int j = 0; j < 6; j++) {  // Limiting to 6 decimal places for simplicity
    fractional_part *= 10;
    int digit = (int)fractional_part;
    str[i++] = digit + '0';
    fractional_part -= digit;
  }

  str[i] = '\0';  // Null terminate the string
}
int main(){
  char str[64];
  double value = 12345.6789;
  double_to_string(value, str);
  printf("%s", str);
}

