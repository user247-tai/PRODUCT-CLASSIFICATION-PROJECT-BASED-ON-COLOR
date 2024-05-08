#include <Servo.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x3F,16,2);
// dinh nghia cac bien dung trong cam bien mau
 // bien cac chan
#define S0 8
#define S1 7
#define S2 11
#define S3 4
#define sensorOut A3
 // bien su dung trong ham
unsigned long time;
int R = 0;
int G = 0;
int B = 0;
int PW = 0;
int colour = 0;
int purple = 0, green = 0, blue = 0;
// dinh nghia cac bien dung trong cam bien hong ngoai
  // bien cac chan
int cambien1 = 3; 
int cambien2 = 5;
int cambien3 = 6;
  // bien su dung trong ham
int gtmacdinh1 = 1, gtmacdinh2 = 1, gtmacdinh3 = 1;

// dinh nghia cac bien dung trong servo
Servo myServo1, myServo2, myServo3;
// khai bao cac bien dung de dem san pham hien thi ra LCD
int a = 0, b = 0, c = 0;

void setup() {
  time = millis();
  // Khai bao cac chan của cam bien mau
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(sensorOut, INPUT);

  // Setting frequency scaling to 20%
  digitalWrite(S0, HIGH);
  digitalWrite(S1, LOW);
  // Khai bao chan cua 3 cam bien hong ngoai
  pinMode(cambien1, INPUT);
  pinMode(cambien2, INPUT);
  pinMode(cambien3, INPUT);
  // Khai bao chan 3 servo
  myServo1.attach(9);
  myServo2.attach(10);
  myServo3.attach(13);
  // Khoi tao LCD
  lcd.init();
  lcd.backlight();
  
  Serial.begin(9600);
  
}

void loop() {
  // Chinh can gat ve gia tri goc quay 90 ban dau de khong vuong vao bang chuyen
  myServo1.write(90); 
  myServo2.write(90);
  myServo3.write(90);
  printLCD();
  colour = 0;
  //sservo();
  colour = readColour();
  int giatri1 = digitalRead(cambien1); //Đọc giá trị digital từ cảm biến 1 và gán vào biến giatri1
  int giatri2 = digitalRead(cambien2); //Đọc giá trị digital từ cảm biến 2 và gán vào biến giatri2
  int giatri3 = digitalRead(cambien3); //Đọc giá trị digital từ cảm biến 3 và gán vào biến giatri3
  
  if (colour==1){Serial.println("purple"); purple=1;} // Nhan biet mau tim
  else if (colour==2){Serial.println("green"); green=1; } // Nhan biet mau xanh la
  else if (colour==3){Serial.println("blue"); blue=1;} // Nhan biet mau xanh duong
  else {Serial.println("no colour detected");} // Khong nhan duoc mau
  if (giatri1 != gtmacdinh1) // doan code tang mau tim
  {
    if (purple==1){
      if (giatri1 == 0)
        {
          a++;
          if ( (unsigned long) (millis() - time) > 3000){ 
            sservo1();
            purple = 0;
            time = millis();
          }
        }
    }
    gtmacdinh1 = giatri1;
  }
  else if (giatri2 != gtmacdinh2) // doan code tang mau xanh la
  {
    if (green==1){
      if (giatri2 == 0)
        {
          b++;
           if ( (unsigned long) (millis() - time) > 6000){ 
            sservo2();
            green = 0;
            time = millis();
          }
        }
    }
    gtmacdinh2 = giatri2;
  }
  else if (giatri3 != gtmacdinh3) // doan code tang xanh duong
  {
    if (blue==1){
      if (giatri3 == 0)
        {
          c++;
          if ( (unsigned long) (millis() - time) > 9000){
            sservo3();
            blue = 0;
            time = millis();
          }
        }
    }
    gtmacdinh3 = giatri3;
  }
}
// 3 ham quay servo
void sservo1(){ // quay khi nhan mau tim
  // xet goc quay
  myServo1.write(0);
  delay(500);
  myServo1.write(90);
}
void sservo2(){ // quay khi nhan mau xanh la
  myServo2.write(0);
  delay(500);
  myServo2.write(90);
}
void sservo3(){ // quay khi nhan mau xanh duong
  myServo3.write(0);
  delay(500);
  myServo3.write(90);
}

int readColour() { // Ham nhan biet mau
  // Setting red photodiodes to be read
  digitalWrite(S2, LOW);
  digitalWrite(S3, LOW);
  PW = pulseIn(sensorOut, LOW);  // Reading the output pulse width
  R = PW;
  // Printing the value on the serial monitor
  Serial.print("R = ");  //printing name
  Serial.print(PW);      //printing RED color pulse width
  Serial.print("  ");
  delay(25);
  // Setting Green photodiodes to be read
  digitalWrite(S2, HIGH);
  digitalWrite(S3, HIGH);
  PW = pulseIn(sensorOut, LOW);  // Reading the output pulse width
  G = PW;
  // Printing the value on the serial monitor
  Serial.print("G = ");  //printing name
  Serial.print(PW);      //printing GREEN color pulse width
  Serial.print("  ");
  delay(25);
  // Setting Blue photodiodes to be read
  digitalWrite(S2, LOW);
  digitalWrite(S3, HIGH);
  PW = pulseIn(sensorOut, LOW);  // Reading the output pulse width
  B = PW;
  // Printing the value on the serial monitor
  Serial.print("B = ");  //printing name
  Serial.print(PW);      //printing BLUE color pulse width
  Serial.print("   colour: ");
  Serial.print(" ");
  //delay(25);
  if (R < 140 && R > 120 && G < 150 && G > 130 && B < 80 && B > 60) {
    colour = 1;  // Purple
  }
  else if (R < 110 && R > 80 && G < 70 && G > 50 && B < 80 && B > 60) {
   colour = 2;  // Green
  }
  else if (R < 155 && R > 130 && G < 75 && G > 50 && B < 40 && B > 15) {
   colour = 3;  // Blue
  }
  return colour;
}

void printLCD() // Ham in ra LCD
{
  lcd.setCursor(0,0);
  lcd.print("  SO LUONG MAU:");
  lcd.setCursor(0,1);
  lcd.print("P:"); // Mau tim
  lcd.print(a);
  lcd.print("  G:"); // Mau xanh la
  lcd.print(b);
  lcd.print("  B:"); // Mau xanh duong
  lcd.print(c);
}
