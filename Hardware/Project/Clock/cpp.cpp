void display_sec_1(int D, int C, int B, int A)
{
  digitalWrite(2, A); //LSB
  digitalWrite(3, B); 
  digitalWrite(4, C); 
  digitalWrite(5, D); //MSB

}
void display_sec_0(int D, int C, int B, int A)
{
  digitalWrite(8, A); //LSB
  digitalWrite(9, B); 
  digitalWrite(10, C); 
  digitalWrite(11, D); //MSB

}
// the setup function runs once when you press reset or power the board
void setup() {
    Serial.begin(9600);
  	pinMode(2, OUTPUT);  
    pinMode(3, OUTPUT);
    pinMode(4, OUTPUT);
    pinMode(5, OUTPUT);
    pinMode(8, OUTPUT);  
    pinMode(9, OUTPUT);
    pinMode(10, OUTPUT);
    pinMode(11, OUTPUT);
    
}

// the loop function runs over and over again forever
void loop() {
  int time_sec = 0;
  while(1){
  	int sec = time_sec;
    int ones_sec = sec%10;
    int tens_sec = (sec - ones_sec)/10;
    int a, b, c, d, e, f, g, h;
    for(int j=3; j>=0; j--){
      int bit0=(ones_sec>>j)&1;
      int bit1=(tens_sec>>j)&1;
      if(j==3){
        a = bit0;
        e = bit1;
      }
      if(j==2){
        b = bit0;
        f = bit1;
      }
      if(j==1){
        c = bit0;
        g = bit1;
      }
      if(j==0){
        d = bit0;
        h = bit1;
      }
    }
    display_sec_0(a, b, c, d);
    display_sec_1(e, f, g, h);
    delay(1000);
    time_sec++;
    if(time_sec >59) time_sec=0;
  }
}
