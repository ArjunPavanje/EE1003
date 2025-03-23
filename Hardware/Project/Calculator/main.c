#include <avr/io.h> 
#include <util/delay.h> 
#include "parseltongue.h"
// #include "funcs.h"
#include <stdlib.h>
#include "string.h"
#include <stdio.h>

#include <avr/eeprom.h>
// ------------------
// TYPEDEFS
typedef uint8_t byte;
#define ADDRESS 0 
// ------------------
// MACROS
// CLR_BIT -> Clears bit number 'Y' on register 'X'
#define CLR_BIT(X, Y) X &= ~_BV(Y)

// SET_BIT -> Sets bit number 'Y' on register 'X'
#define SET_BIT(X, Y) X |= _BV(Y)

// LCD_RS -> R/S (Register Select)
#define LCD_RS 0 // PB0

// LCD_E -> LCD Enable pin
#define LCD_E 1 // PB1

// DATA PINS
#define DB4 2 // pin for DB4
#define DB5 3 // pin for DB5
#define DB6 4 // pin for DB6
#define DB7 5 // pin for DB7

// CLEAR_DISPLAY -> Instruction for clearing display
#define CLEAR_DISPLAY 0x01

// BUTTON_THRESHOLD -> Sets the button delay between taps
// equivalent to BUTTON_THRESHOLD*50 ms
#define BUTTON_THRESHOLD 10

// N_BUTTONS -> Number of active buttons
#define N_BUTTONS 5


/* LCD FUNCTIONS */
void pulse_enable_line(){
  SET_BIT(PORTB, LCD_E); // take LCD enable line high
  _delay_us(40); // wait 40 microseconds
  CLR_BIT(PORTB, LCD_E); // take LCD enable line low
}

void send_nibble(byte data){
  PORTB &= 0xC3; // 1100.0011 = clear 4 data lines
  if (data & _BV(4)) SET_BIT(PORTB, DB4);
  if (data & _BV(5)) SET_BIT(PORTB, DB5);
  if (data & _BV(6)) SET_BIT(PORTB, DB6);
  if (data & _BV(7)) SET_BIT(PORTB, DB7);
  pulse_enable_line(); // clock 4 bits into controller
}

void send_byte(byte data){
  send_nibble(data); // send upper 4 bits
  send_nibble(data << 4); // send lower 4 bits
  CLR_BIT(PORTB, 5); // turn off PIN 13 (Clk i guess)
}

void lcd_cmd(byte cmd){
  CLR_BIT(PORTB, LCD_RS); // R/S line 0 = command data
  send_byte(cmd); // send it
}

void lcd_char(byte ch){
  SET_BIT(PORTB, LCD_RS); // R/S line 1 = character data
  send_byte(ch); // send it
}

void lcd_init(){
  lcd_cmd(0x33); // initialize controller
  lcd_cmd(0x32); // set to 4-bit input mode
  lcd_cmd(0x28); // 2 line, 5x7 matrix
  lcd_cmd(0x0E); // turn cursor off (0x0E to enable)
  lcd_cmd(0x06); // cursor direction = right
  lcd_cmd(0x01); // start with clear display
  _delay_ms(3); // wait for LCD to initialize
}

void lcd_clear() {
  lcd_cmd(CLEAR_DISPLAY);
  _delay_ms(3); // wait for LCD to process command
}

void lcd_msg(const char *text){
  while (*text) 
    lcd_char(*text++); // send char & update char pointer
}

void lcd_double(double data){
  char* st; // save enough space for result
  snprintf(st, 7, "%lf", data);
  while(*st) lcd_char(*st++);
  //lcd_msg(st); // display in on LCD
}

void lcd_int(int data){
  char st[8] = ""; // save enough space for result
  itoa(data,st,10); // convert integer to ascii 
  lcd_msg(st); // display in on LCD
}

void display_biline(int pos1, char* buf1, int pos2, char* buf2){
  lcd_clear();

  lcd_cmd(0x80);
  if(pos1 > 16) lcd_msg(buf1 + pos1 - 15);
  else lcd_msg(buf1);

  lcd_cmd(0xC0);
  if(pos2 > 16) lcd_msg(buf2 + pos2 - 15);
  else lcd_msg(buf2);

  lcd_cmd(0x80);

  for(int i = 0; i < (pos1 > 16 ? 15: pos1); i++) lcd_cmd(0x14);
}
/* UTILITY FUNCTIONS */
void clear_buf(int* len, char* buf){
  for(int i = 0; i < *len; i++) buf[i] = '\0';
  *len = 0;
}

// Function to count unmatched opening brackets
int count_unmatched_brackets(char* expr) {
  int count = 0;
  for(int i = 0; expr[i] != '\0'; i++) {
    if(expr[i] == '(') count++;
    else if(expr[i] == ')') count--;
  }
  return count;
}
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
/* BUTTON FUNCTIONS */
void button_listener(int* button_x, int* button_y, int* time_lapsed){
  if(*time_lapsed > BUTTON_THRESHOLD) {
    *button_x = -1;
    *button_y = -1;
  }

  if((*button_x != -1) && (*time_lapsed < BUTTON_THRESHOLD)) return;

  for(int k = 0; k < N_BUTTONS; k++){
    CLR_BIT(PORTD, (k + 2));

    for(int i = 0; i < N_BUTTONS; i++){
      if(!(PINC & (1 << (i)))){
        *button_x = k;
        *button_y = i;
        *time_lapsed = 0;
        SET_BIT(PORTD, (k + 2));
        return;
      }
    }

    SET_BIT(PORTD, (k + 2));
  }

  /*
    *button_x = -1;
    *button_y = -1;
    */
  return;
}

// Add this function to convert tokens to their display names
void get_token_display(char token, char* display) {
  switch(token) {
    case 's': strcpy(display, "sin"); break;
    case 'c': strcpy(display, "cos"); break;
    case 't': strcpy(display, "tan"); break;
    case '@': strcpy(display, "arcsin"); break;
    case '$': strcpy(display, "arccos"); break;
    case '#': strcpy(display, "arctan"); break;
    case 'l': strcpy(display, "ln"); break;
    case 'k': strcpy(display, "log10"); break;
    case 'p': strcpy(display, "pi"); break;
    case 'e': strcpy(display, "e"); break;
    default: display[0] = token; display[1] = '\0'; break;
  }
}

// Add this function to create a display string from the expression buffer
void create_display_string(char* expr_buf, char* display_buf) {
  int i = 0;
  int j = 0;

  while(expr_buf[i] != '\0') {
    char token_display[10] = {0}; // Buffer for the display text of a token
    get_token_display(expr_buf[i], token_display);

    // Copy the display text to the display buffer
    int k = 0;
    while(token_display[k] != '\0') {
      display_buf[j++] = token_display[k++];
    }

    i++;
  }

  display_buf[j] = '\0'; // Null-terminate the display buffer
}
char button_map(int button_x, int button_y){
  // 0 - 9 -> {(0, 0), (0, 1), .. (0, 4)} U {(1, 0), (1, 1), .. (1, 4)}
  if(button_y == 0 || button_y == 1){
    return ((char) (((button_y*5) + button_x + 1) + '0'));
  }

  return ' ';
}


char jenson_button(int button_x, int button_y, int mode){
  if (mode == 0){
    char button_map1[5][5] = {
      { 's', 'c', 't', '<', 'g'}, // yellow
      { '5', '6', '7', '8', '9'}, // blue

      { '+', '-', '*', '/', '^'}, // red
      { '0', '1', '2', '3', '4'}, // yellow
      { '=', '.', 'M', '<', '_'}, // white
    };
    return button_map1[button_y][button_x];
  }
  else{
    char button_map2[5][5] = {
      { '@', '$', '#', 'l', 'G'}, // yellow
      { '5', '6', '7', '8', '9'}, // blue

      { '!', 'p', 'e', 'l', 'G'}, // red
      { '0', '1', '2', '3', '4'}, // yellow
      { '=', '.', 'M', '<', '_'}, // white
    }; 
    return button_map2[button_y][button_x];
  }
}


int main(void){

  // Disable ADC (Convert Analog -> Digital)
  ADCSRA &= ~(1 << ADEN);

  DDRC = 0x00;
  PORTC = 0xFF; // Setting input pull up

  // use PortB for LCD interface
  DDRB = 0xFF; // 1111.1111; set PB0-PB7 as outputs	 

  // use PortD for Buttons
  DDRD = 0xFF; // 0000.0000; set PD0-PD7 as inputs
  PORTD = 0xFF;

  lcd_init(); // initialize LCD controller
  int button_x = -1;
  int button_y = -1;
  int debounce = 0;
  int is_answer_loop = 0;

  char buf1[64] = {'\0'};
  int pos1 = 0;

  char display_buf1[128] = {'\0'};  // Display buffer for human-readable expression
  int display_pos1 = 0;  

  char buf2[64] = {'\0'};
  int pos2 = 0;

  int mode = 0;

  double ans = 0;
  double memory_value = 0;
  while(1){
    button_listener(&button_x, &button_y, &debounce);

    int in_function = 0;
    int function_start_pos = -1;    

    if(button_x != -1 && debounce == 0){
      char token = jenson_button(button_x, button_y, mode);
      if (token == 'M'){
        if(mode == 0) mode = 1;
        else mode = 0;
        debounce +=1;
        continue;
      }

      if(token == 'B') {
        // Determine which bracket to insert based on context
        char bracket_to_insert;

        // If buffer is empty or last character is an operator or opening bracket, insert '('
        if(pos1 == 0 || 
          buf1[pos1-1] == '+' || buf1[pos1-1] == '-' || 
          buf1[pos1-1] == '*' || buf1[pos1-1] == '/' || 
          buf1[pos1-1] == '^' || buf1[pos1-1] == '(') {
          bracket_to_insert = '(';
        }
        // If last character is a digit, closing bracket, or a constant (e, pi), insert ')'
        else if(isdigit(buf1[pos1-1]) || buf1[pos1-1] == ')' || 
          buf1[pos1-1] == 'e' || buf1[pos1-1] == 'p') {
          bracket_to_insert = ')';
        }
        // Default to opening bracket in other cases
        else {
          bracket_to_insert = '(';
        }

        // Add the selected bracket to the buffer
        buf1[pos1++] = bracket_to_insert;
        display_buf1[display_pos1++] = bracket_to_insert;

        // Null terminate the buffers
        buf1[pos1] = '\0';
        display_buf1[display_pos1] = '\0';

        debounce += 1;
        continue;
      }

      /*
      if(token == 'm'){
        char memory_str[64];
        //dtostrf(memory_value, 16, 5, memory_str);
        //sprintf(memory_str, "%lf", memory_value);
        double_to_string(memory_value, memory_str);
        //int len = 0;
        for(int w=0; memory_str[w]!='\0'; w++){
          display_buf1[display_pos1] = memory_str[w];
          buf1[pos1] = memory_str[w];
          display_pos1 += 1;
          pos1+=1;
          //len+=1;
        }

        buf1[pos1]='\0';
        display_buf1[display_pos1]='\0';
        debounce +=1;
        continue;
      }
/*
      if(token == 'm'){
        // Convert memory value to string
        char memory_str[20];
        dtostrf(memory_value, 16, 5, memory_str);

        // Insert memory value at current position in the expression
        strcat(buf1 + pos1, memory_str);
        pos1 += strlen(memory_str);
        buf1[pos1] = '\0';

        // Update display buffer
        create_display_string(buf1, display_buf1);
        display_pos1 = strlen(display_buf1);

        debounce += 1;
        continue;
      }

*/
      /*if(token == '='){
        is_answer_loop = 1;
        lcd_clear();

        ans = evaluate(buf1);
        memory_value = ans; // Store the answer in memory_value 
        dtostrf(ans, 16, 5, buf2);
        pos2 = 16;
        debounce += 1;
        _delay_ms(50);
        continue;
      }*/

      if(token == '='){
        is_answer_loop = 1;
        lcd_clear();

        // Check for unmatched opening brackets and add closing brackets if needed
        int unmatched = count_unmatched_brackets(buf1);
        for(int i = 0; i < unmatched; i++) {
          buf1[pos1++] = ')';
          display_buf1[display_pos1++] = ')';
        }
        buf1[pos1] = '\0';
        display_buf1[display_pos1] = '\0';

        ans = evaluate(buf1);
        memory_value = ans; // Store the answer in memory_value 
        dtostrf(ans, 16, 5, buf2);
        //if(ans > 1e15) sprintf(buf2, "%16.*e", ans);
        //else dtostrf(ans, 16, 5, buf2);        
        pos2 = 16;
        debounce += 1;
        _delay_ms(50);
        continue;
      }
      if(token == 'g'){
        if(is_answer_loop) eeprom_write_block((const void*) &ans, (void*) ADDRESS, sizeof(double));
        debounce += 1;
        _delay_ms(50);
        continue;
      }      
      if(is_answer_loop){
        clear_buf(&pos1, buf1);
        clear_buf(&display_pos1, display_buf1);
        clear_buf(&pos2, buf2);
        is_answer_loop = 0;
      }


      if(token == '<'){
        if(pos1 > 0) {
          // Get the display length of the character being deleted
          char token_display[10] = {0};
          get_token_display(buf1[pos1-1], token_display);
          int display_len = strlen(token_display);

          // Remove the character from buf1
          buf1[pos1-1] = '\0';
          pos1--;

          // Remove the corresponding display text from display_buf1
          display_pos1 -= display_len;
          display_buf1[display_pos1] = '\0';
        }
      }
      else if(token == '_'){
        clear_buf(&display_pos1, display_buf1);
        clear_buf(&pos1, buf1);
        clear_buf(&pos2, buf2);
      }      
      // Check if token is a function
      else if (token == 's' || token == 'c' || token == 't' || 
        token == '@' || token == '$' || token == '#' || 
        token == 'l' || token == 'k') {

        // Add the function token
        buf1[pos1++] = token;

        // Automatically add opening bracket
        buf1[pos1++] = '(';

        // Set the function flag
        in_function = 1;
        function_start_pos = pos1;

        // Update display buffer
        char token_display[10] = {0};
        get_token_display(token, token_display);
        strcpy(display_buf1 + display_pos1, token_display);
        display_pos1 += strlen(token_display);

        // Add opening bracket to display
        display_buf1[display_pos1++] = '(';
      }
      // Check if we need to close a bracket (when an operator is pressed)
      else if (in_function && (token == '+' || token == '-' || 
        token == '*' || token == '/' || token == '=' ||
        token == '^')) {

        // Add closing bracket before the operator
        buf1[pos1++] = ')';
        display_buf1[display_pos1++] = ')';

        // Reset function flag
        in_function = 0;

        // Now add the operator
        buf1[pos1++] = token;
        display_buf1[display_pos1++] = token;
      } 
      else if(token == 'G'){
        double mem = 0;
        eeprom_read_block((void*) &mem, (const void*) ADDRESS, sizeof(double));      
        char str[64];
        sprintf(str, "%lf", mem);
        //double_to_string(mem, str);
        for(int w=0; str[w]!='\0'; w++){
          buf1[pos1++] = str[w];
          display_buf1[display_pos1++] = str[w];
        }
      }
      else {
        // Regular token handling
        buf1[pos1++] = token;

        // Update display buffer
        char token_display[10] = {0};
        get_token_display(token, token_display);
        strcpy(display_buf1 + display_pos1, token_display);
        display_pos1 += strlen(token_display);
      }

      // Null terminate the buffers
      buf1[pos1] = '\0';
      display_buf1[display_pos1] = '\0';
    }
    //    if(button_x != -1 && debounce == 0){
    //      char token = jenson_button(button_x, button_y, mode);
    //
    //      if (token == 'M'){
    //        if(mode == 0) mode = 1;
    //        else mode = 0;
    //        debounce +=1;
    //        continue;
    //      }
    //
    //      if(token == '='){
    //        is_answer_loop = 1;
    //        lcd_clear();
    //
    //        ans = evaluate(buf1);
    //        dtostrf(ans, 16, 5, buf2);
    //        pos2 = 16;
    //        debounce += 1;
    //        _delay_ms(50);
    //        continue;
    //      }
    //
    //      if(is_answer_loop){
    //        clear_buf(&pos1, buf1);
    //        clear_buf(&display_pos1, display_buf1);
    //        clear_buf(&pos2, buf2);
    //        is_answer_loop = 0;
    //      }
    //
    //      if(token == '<'){
    //        if(pos1 > 0) {
    //          // Get the display length of the character being deleted
    //          char token_display[10] = {0};
    //          get_token_display(buf1[pos1-1], token_display);
    //          int display_len = strlen(token_display);
    //
    //          // Remove the character from buf1
    //          buf1[pos1-1] = '\0';
    //          pos1--;
    //
    //          // Remove the corresponding display text from display_buf1
    //          display_pos1 -= display_len;
    //          display_buf1[display_pos1] = '\0';
    //        }
    //      }
    //      else{
    //        // Add the token to buf1
    //        buf1[pos1++] = token;
    //        buf1[pos1] = '\0';
    //
    //        // Add the display text to display_buf1
    //        char token_display[10] = {0};
    //        get_token_display(token, token_display);
    //
    //        strcpy(display_buf1 + display_pos1, token_display);
    //        display_pos1 += strlen(token_display);
    //      }
    //    }

    lcd_clear();
    // Display the human-readable expression and result
    display_biline(display_pos1, display_buf1, pos2, buf2);
    debounce += 1;    

    _delay_ms(50);     // set animation speed
  }

}
