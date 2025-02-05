void sevenseg(int a,int b,int c,int d,int e,int f,int g)
{
  digitalWrite(2, a); 
  digitalWrite(3, b); 
  digitalWrite(4, c); 
  digitalWrite(5, d); 
  digitalWrite(6, e); 
  digitalWrite(7, f);     
  digitalWrite(8, g); 
}
void setup() 
{
  pinMode(2, OUTPUT);  
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);            
}
void loop() 
{
  // 1
  sevenseg(1,0,0,1,1,1,1);
  delay(1000);
  // 2
  sevenseg(0,0,1,0,0,1,0);
  delay(1000);
  // 3
  sevenseg(0,0,0,0,1,1,0);
  delay(1000);
  // 4
  sevenseg(1,0,0,1,1,0,0);
  delay(1000);
  // 5
  sevenseg(0,1,0,0,1,0,0);
  delay(1000);
  // 6
  sevenseg(0,1,0,0,0,0,0);
  delay(1000);
  // 7
  sevenseg(0,0,0,1,1,1,1);
  delay(1000);
  // 8
  sevenseg(0,0,0,0,0,0,0);
  delay(1000);
  // 9
  sevenseg(0,0,0,0,1,0,0);
  delay(1000);
  // 0
  sevenseg(0,0,0,0,0,0,1);
  delay(1000);

  /*
     sevenseg(1,0,0,1,1,1,1); 
     sevenseg(1,0,0,1,1,1,1); 
     sevenseg(1,0,0,1,1,1,1); 
     sevenseg(1,0,0,1,1,1,1); 
     */
}
