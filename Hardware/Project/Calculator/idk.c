
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
}/* BUTTON FUNCTIONS */
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
      { 's', 'c', 't', '^', 'l'}, // yellow
      { '5', '6', '7', '8', '9'}, // blue

      { '+', '-', '*', '/', '<'}, // red
      { '0', '1', '2', '3', '4'}, // yellow
      { '=', '.', 'M', '(', ')'}, // white
    };
    return button_map1[button_y][button_x];
  }
  else{
     char button_map2[5][5] = {
      { '@', '$', '#', 'p', 'k'}, // yellow
      { '5', '6', '7', '8', '9'}, // blue

      { '!', '-', 'p', 'e', '<'}, // red
      { '0', '1', '2', '3', '4'}, // yellow
      { '=', '.', 'M', '(', ')'}, // white
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

  char buf2[64] = {'\0'};
  int pos2 = 0;
  int mode = 0;

  double ans = 0;

  while(1){
    button_listener(&button_x, &button_y, &debounce);
    lcd_clear();

    if(button_x != -1 && debounce == 0){
      char token = jenson_button(button_x, button_y, mode);
      if (token == 'M'){
        if(mode == 0) mode = 1;
        else mode = 0;
        debounce +=1;
        continue;
      }
      if(token == '='){
        is_answer_loop = 1;
        lcd_clear();

        ans = evaluate(buf1);
        dtostrf(ans, 16, 5, buf2);
        pos2 = 16;
        debounce += 1;
        _delay_ms(50);
        continue;
      }
      if(is_answer_loop){
        clear_buf(&pos1, buf1);
        clear_buf(&pos2, buf2);
        is_answer_loop = 0;
      }
      if(token == '<'){
        buf1[pos1 - 1] = '\0';
        pos1 = pos1 - 1;
      }
      else{
        buf1[pos1] = token;
        pos1 += 1;
      }
    }

    lcd_clear();
    display_biline(pos1, buf1, pos2, buf2);
    debounce += 1;    

    _delay_ms(50);     // set animation speed
  }
}
