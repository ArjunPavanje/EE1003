#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
//#include <funcs.h>
// Define stack size
#define ADDRESS 0
#define PI 3.14159265358979323846
#define E 2.718281828459045 
#define MAX_DIGITS 10000  // The maximum number of digits we can store
#define H 0.01
// Function to calculate factorial of a number and return it as a double
double factorial(int n) {
  double result = 1.0;  // Initialize result as 1.0

  // Multiply each number from 2 to n
  for (int i = 2; i <= n; i++) {
    result *= i;
  }
  return result;
}

double fast_inv_sqrt(double x){
  x = (float) x;
  long i;
  float x2, y;
  const float threehalfs = 1.5F;

  x2 = x * 0.5F;
  y  = x;
  i  = * ( long * ) &y;
  i  = 0x5f3759df - ( i >> 1 );
  y  = * ( float * ) &i;
  y  = y * ( threehalfs - ( x2 * y * y ) );

  return (double) y;
}

// Function to normalize angle to [0, 2π)
double normalize_angle(double angle) {
  // First, get the remainder when divided by 2π
  while (angle >= 2*PI) {
    angle -= 2*PI;
  }
  while (angle < 0) {
    angle += 2*PI;
  }
  return angle;
}
// Function to calculate sine using RK4 method
double sin_rk4(double x_target) {
  double x, y, z;
  double k1, k2, k3, k4, l1, l2, l3, l4;

  // Normalize the target angle to [0, 2π)
  x_target = normalize_angle(x_target);

  // Initial conditions for y = sin(x)
  x = 0.0;    // Starting x value
  y = 0.0;    // y(0) = 0
  z = 1.0;    // y'(0) = 1

  // RK4 method to solve the differential equation
  while(x < x_target) {
    // Ensure we don't overshoot the target
    if (x + H > x_target) {
      // Adjust the last step size to hit x_target exactly
      double last_h = x_target - x;

      // Calculate k values for y
      k1 = last_h * z;
      l1 = -last_h * (y);

      k2 = last_h * (z + l1/2);
      l2 = -last_h * (y + k1/2);

      k3 = last_h * (z + l2/2);
      l3 = -last_h * (y + k2/2);

      k4 = last_h * (z + l3);
      l4 = -last_h * (y + k3);

      // Update y and z
      y = y + (k1 + 2*k2 + 2*k3 + k4)/6;
      z = z + (l1 + 2*l2 + 2*l3 + l4)/6;

      // Update x to target
      x = x_target;
    } else {
      // Regular step
      // Calculate k values for y
      k1 = H * (z);
      l1 = -H * (y);

      k2 = H * (z + l1/2);
      l2 = -H * (y + k1/2);

      k3 = H * (z + l2/2);
      l3 = -H * (y + k2/2);

      k4 = H * (z + l3);
      l4 = -H * (y + k3);

      // Update y and z
      y = y + (k1 + 2*k2 + 2*k3 + k4)/6;
      z = z + (l1 + 2*l2 + 2*l3 + l4)/6;

      // Update x
      x = x + H;
    }
  }

  return y;
}

// Define the differential equation: dy/dx = w * y / x
double f(double x, double y, double w) {
  return w * y / x;
}

// Function to calculate x^w using RK4 method

// Natural log using RK4 for dy/dx = 1/x
double ln_rk4(double x) {
  if (x <= 0) return 0;
  if (x < 1) return -ln_rk4(1/x);

  double x0 = 1.0, y = 0.0;
  int steps = (int)((x - x0) / H);

  for (int i = 0; i < steps; i++) {
    double k1 = H / x0;
    double k2 = H / (x0 + H/2);
    double k3 = H / (x0 + H/2);
    double k4 = H / (x0 + H);

    y += (k1 + 2*k2 + 2*k3 + k4) / 6;
    x0 += H;
  }

  return y;
}
double log10_rk4(double x) {
  // Calculate log base 10 using the change of base formula
  return ln_rk4(x) / ln_rk4(10.0);
}
double power(double x, double w) {
  if (x == 0) return 0;  // Handle special case
  if (w == 0) return 1;  // x^0 = 1

  // Handle integer exponents exactly for better precision
  if (w == (int)w && w > 0 && w <= 100) {
    int sign = 1;
    if (x < 0 && (int)w % 2 == 1) {
      sign = -1;
    }

    double absX = x>0?x:-x;
    double result = 1.0;
    for (int i = 0; i < (int)w; i++) {
      result *= absX;
    }
    return sign * result;
  }

  // Handle negative base
  int sign = 1;
  if (x < 0) {
    // Check if exponent is an integer
    if (w != (int)w) {
      return 0.0;
    }

    // Determine sign based on whether exponent is odd or even
    if ((int)w % 2 == 1) {
      sign = -1;
    }

    // Use absolute value for calculation
    x = x>0?x:-x;
  }

  // Handle negative exponents
  int negativeExponent = 0;
  if (w < 0) {
    w = w>0?w:-w;
    negativeExponent = 1;
  }

  // Continue with RK4 method for non-integer exponents
  double x0 = 1.0;
  double y = 1.0;  // y(1) = 1

  // Calculate number of steps needed
  int steps = (int)((x - x0) / H);
  if (x < 1.0) {
    steps = (int)((x0 - x) / H);
    steps = -steps;  // Negative steps for x < 1
  }

  // Apply RK4 method
  for (int i = 0; i < steps; i++) {
    double k1 = H * f(x0, y, w);
    double k2 = H * f(x0 + 0.5 * H, y + 0.5 * k1, w);
    double k3 = H * f(x0 + 0.5 * H, y + 0.5 * k2, w);
    double k4 = H * f(x0 + H, y + k3, w);

    y += (k1 + 2 * k2 + 2 * k3 + k4) / 6;
    x0 += H;
  }

  // Handle any remaining fraction
  double remaining = x - x0;
  if (remaining > 0) {
    double k1 = remaining * f(x0, y, w);
    double k2 = remaining * f(x0 + 0.5 * remaining, y + 0.5 * k1, w);
    double k3 = remaining * f(x0 + 0.5 * remaining, y + 0.5 * k2, w);
    double k4 = remaining * f(x0 + remaining, y + k3, w);

    y += (k1 + 2 * k2 + 2 * k3 + k4) / 6;
  }

  // Apply negative exponent if needed
  if (negativeExponent) {
    y = 1.0 / y;
  }

  // Apply sign
  return sign * y;
}

// Arctan using RK4 for dy/dx = 1/(1+x²)
double arctan_rk4(double x) {
  double x0 = 0.0, y = 0.0;
  int steps = x >= 0 ? (int) (x / H): (int) (-x / H); 
  double step_dir = (x >= 0) ? 1 : -1;

  for (int i = 0; i < steps; i++) {
    double k1 = H / (1 + x0*x0);
    double k2 = H / (1 + (x0 + H/2)*(x0 + H/2));
    double k3 = H / (1 + (x0 + H/2)*(x0 + H/2));
    double k4 = H / (1 + (x0 + H)*(x0 + H));

    y += step_dir * (k1 + 2*k2 + 2*k3 + k4) / 6;
    x0 += step_dir * H;
  }

  return y;
}

// Arcsin using RK4 for dy/dx = 1/sqrt(1-x²)
double arcsin_rk4(double x) {
  if (x < -1 || x > 1) return 0;

  double x0 = 0.0, y = 0.0;
  int steps = x >= 0 ? (int) (x / H): (int) (-x / H); 
  double step_dir = (x >= 0) ? 1 : -1;

  for (int i = 0; i < steps; i++) {
    double k1 = H * fast_inv_sqrt(1 - x0*x0);
    double k2 = H * fast_inv_sqrt(1 - (x0 + step_dir*H/2)*(x0 + step_dir*H/2));
    double k3 = H * fast_inv_sqrt(1 - (x0 + step_dir*H/2)*(x0 + step_dir*H/2));
    double k4 = H * fast_inv_sqrt(1 - (x0 + step_dir*H)*(x0 + step_dir*H));

    y += step_dir * (k1 + 2*k2 + 2*k3 + k4) / 6;
    x0 += step_dir * H;
  }

  return y;
}

double arccos_rk4(double x){
  return ((PI/2) - arcsin_rk4(x));
}




#define MAX 100

// Stack structure to hold operands
typedef struct {
  int top;
  double items[MAX];
} Stack;

// Function to initialize stack
void initStack(Stack *s) {
  s->top = -1;
}

// Function to check if the stack is empty
int isEmpty(Stack *s) {
  return s->top == -1;
}

// Function to push an element onto the stack
void push(Stack *s, double item) {
  if (s->top < (MAX - 1)) {
    s->items[++(s->top)] = item;
  }
}

// Function to pop an element from the stack
double pop(Stack *s) {
  if (!isEmpty(s)) {
    return s->items[(s->top)--];
  }
  return -1; // return -1 if stack is empty
}

// Function to peek the top element of the stack
double peek(Stack *s) {
  if (!isEmpty(s)) {
    return s->items[s->top];
  }
  return -1; // return -1 if stack is empty
}

// Function to check if the character is an operator
int isOperator(char c) {
  return (c == '+' || c == '-' || c == '*' || c == '/' || c == '^');
}

// Function to check precedence of operators
int precedence(char c) {
  if (c == '+' || c == '-') {
    return 1;
  } else if (c == '*' || c == '/') {
    return 2;
  } else if (c == '^') {
    return 3;
  }else if (c == '!') {
    return 4;  // Highest precedence
  }
  return 0;
}

// Function to evaluate postfix expressions
double evaluatePostfix(char *postfix) {
  Stack s;
  initStack(&s);

  int i = 0;
  while (postfix[i] != '\0') {
    // Skip spaces
    if (postfix[i] == ' ') {
      i++;
      continue;
    }

    if (isdigit(postfix[i]) || (postfix[i] == '-' && (isdigit(postfix[i+1]) || postfix[i+1] == '.'))) {
      // Parse number (including negative numbers)
      double num = 0;
      int negative = 0;

      if (postfix[i] == '-') {
        negative = 1;
        i++;
      }

      // Parse integer part
      while (isdigit(postfix[i])) {
        num = num * 10 + (postfix[i] - '0');
        i++;
      }

      // Parse decimal part
      if (postfix[i] == '.') {
        i++;
        double fraction = 0.1;
        while (isdigit(postfix[i])) {
          num += (postfix[i] - '0') * fraction;
          fraction *= 0.1;
          i++;
        }
      }

      if (negative) num = -num;
      push(&s, num);
    } else if (isOperator(postfix[i])) {
      // Process binary operators
      double operand2 = pop(&s);
      double operand1 = pop(&s);
      double result;

      switch (postfix[i]) {
        case '+': result = operand1 + operand2; break;
        case '-': result = operand1 - operand2; break;
        case '*': result = operand1 * operand2; break;
        case '/': result = operand1 / operand2; break;
        case '^': result = power(operand1, operand2); break;
      }

      push(&s, result);
      i++;
    } else if (postfix[i] == 'l') {
      // Process natural logarithm function
      double operand = pop(&s);
      push(&s, ln_rk4(operand));
      i++;
    }else if (postfix[i] == 'k') {
      // Process natural logarithm function
      double operand = pop(&s);
      push(&s, log10_rk4(operand));
      i++;
    } 
    else if (postfix[i] == 's') {
      // Process sine function
      double operand = pop(&s);
      push(&s, sin_rk4(operand));
      i++;
    } else if (postfix[i] == 'c') {
      // Process cosine function
      double operand = pop(&s);
      push(&s, sin_rk4(PI/2 - operand));
      i++;
    } else if (postfix[i] == 't') {
      // Process tangent function
      double operand = pop(&s);
      push(&s, sin_rk4(operand)/sin_rk4(PI/2 - operand));
      i++;
    } else if (postfix[i] == 'p') {
      // Push pi value
      push(&s, PI);
      i++;
    } else if (postfix[i] == 'e') {
      push(&s, E);
      i++;
    }else if (postfix[i] == 'G'){
      //double mem = 0;
      //eeprom_read_block((void*) &mem, (const void*) ADDRESS, sizeof(double)); 
      push(&s, 6);
      i++;
    } else if (postfix[i] == '@') {
      // Process sine inverse function
      double operand = pop(&s);
      push(&s, arcsin_rk4(operand));
      i++;
    } else if (postfix[i] == '$') {
      // Process cosine inverse function
      double operand = pop(&s);
      push(&s, arccos_rk4(operand));
      i++;
    } else if (postfix[i] == '#') {
      // Process tangent inverse function
      double operand = pop(&s);
      push(&s, arctan_rk4(operand));
      i++;
    } else if (postfix[i] == '!') {
      double operand = pop(&s);
      push(&s, factorial((int)operand));
      i++;
    } else {
      // Skip other characters
      i++;
    }
  }

  // The final result should be on top of the stack
  return pop(&s);
}

// Function to handle infix to postfix conversion
void infixToPostfix(char *infix, char *postfix) {
  Stack ops;
  int k = 0;
  int i = 0;
  int length = 0;
  for(int w=0; infix[w]!='\0'; w++){
    length+=1;
  }

  // Initialize stack
  initStack(&ops);

  while (i < length) {
    char token = infix[i];

    // Handle negative numbers at the beginning or after an operator or opening parenthesis
    if (token == '-' && (i == 0 || infix[i-1] == '(' || isOperator(infix[i-1]))) {
      // Check if it's a negative number
      if (i+1 < length && (isdigit(infix[i+1]) || infix[i+1] == '.')) {
        // It's a negative number, copy the negative sign
        postfix[k++] = '-';
        i++;

        // Parse and add the number (including decimal point)
        while (i < length && (isdigit(infix[i]) || infix[i] == '.')) {
          postfix[k++] = infix[i++];
        }
        postfix[k++] = ' '; // separate numbers
        continue;
      }
      // Otherwise, it's a unary minus, handle as 0-operand
    }

    // Handle functions like sin, cos, tan, ln
    if ((token == 's' || token == 'c' || token == 't' || token == 'l' || token == '@' || token =='#' || token == '$'|| token == 'k') && i + 1 < length && infix[i + 1] == '(') {
      char func = token; // Save the function type
      i += 2; // Skip the function letter and '('

      // Find the matching closing parenthesis
      int parenthesisCount = 1;
      int start = i;

      while (i < length && parenthesisCount > 0) {
        if (infix[i] == '(') parenthesisCount++;
        else if (infix[i] == ')') parenthesisCount--;
        if (parenthesisCount > 0) i++;
      }

      // Extract the argument expression
      char subExpr[MAX];
      strncpy(subExpr, infix + start, i - start);
      subExpr[i - start] = '\0';

      // Convert the sub-expression to postfix and append it
      char subPostfix[MAX];
      infixToPostfix(subExpr, subPostfix);

      // Append the sub-expression's postfix to our postfix
      for (int j = 0; subPostfix[j] != '\0'; j++) {
        postfix[k++] = subPostfix[j];
      }

      // Append the function operator
      postfix[k++] = func;
      postfix[k++] = ' ';

      i++; // Skip the closing ')'
    } else if (token == 'p') {
      // Handle 'p' as pi constant
      postfix[k++] = 'p';
      postfix[k++] = ' ';
      i++;
    }else if (token == 'e') {
      // Handle 'p' as pi constant
      postfix[k++] = 'e';
      postfix[k++] = ' ';
      i++;
    }
    else if (token == 'G') {
      // Handle 'p' as pi constant
      postfix[k++] = 'G';
      postfix[k++] = ' ';
      i++;
    } 
    else if (isdigit(token) || token == '.') {
      // Handle numbers that start with a decimal point (e.g., .5)
      if (token == '.' && (i == 0 || !isdigit(infix[i-1]))) {
        postfix[k++] = '0'; // Add leading 0 before decimal point
      }

      // Parse and add the number (including decimal point)
      while (i < length && (isdigit(infix[i]) || infix[i] == '.')) {
        postfix[k++] = infix[i++];
      }
      postfix[k++] = ' '; // separate numbers
    } else if (token == '(') {
      push(&ops, token);
      i++;
    } else if (token == ')') {
      while (!isEmpty(&ops) && peek(&ops) != '(') {
        postfix[k++] = pop(&ops);
        postfix[k++] = ' ';
      }
      pop(&ops); // Pop '(' from stack
      i++;
    } else if (token == '!') {
      while (!isEmpty(&ops) && precedence(peek(&ops)) >= precedence('!')) {
        postfix[k++] = pop(&ops);
        postfix[k++] = ' ';
      }
      push(&ops, token);
      i++;
    }

    else if (isOperator(token)) {
      while (!isEmpty(&ops) && precedence(peek(&ops)) >= precedence(token) && peek(&ops) != '(') {
        postfix[k++] = pop(&ops);
        postfix[k++] = ' ';
      }
      push(&ops, token);
      i++;
    } else {
      // Skip other characters
      i++;
    }
  }

  // Pop all remaining operators from the stack
  while (!isEmpty(&ops)) {
    postfix[k++] = pop(&ops);
    postfix[k++] = ' ';
  }

  postfix[k] = '\0'; // Null-terminate the postfix expression
}
// Function to evaluate an infix expression
double evaluate(char infix[MAX]) {
  char postfix[MAX];
  infixToPostfix(infix, postfix);

  // For debugging
  printf("Infix: %s\n", infix);
  printf("Postfix: %s\n", postfix);

  double result = evaluatePostfix(postfix);
  return result;
}
int main(){
  printf("%lf", evaluate("G*2"));
}
