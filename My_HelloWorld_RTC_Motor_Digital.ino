// include the library code:
#include <Wire.h>
#include <DS3231.h>
#include <LiquidCrystal.h>


// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
RTClib myRTC;


// I/O pins are initialized along with function variables
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
int hour, minute, second;
bool on = true;
int count = 0;
int result; 
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// Define LCD backlight pin and motor control pins
#define LCD_BACKLIGHT_PIN 10
#define ENABLE 8
#define DIRA 6 //clockwise direction
#define DIRB 7 //counter clockwise direction

int buttonPin = 13;
int i;
unsigned long previousMillis = 0;
long interval = 5000;


void setup() {
// set up the LCD's number of columns and rows:
  analogWrite(LCD_BACKLIGHT_PIN, 255);
  lcd.begin(16, 2);

// Initialize write to serial command prompt
  Serial.begin(9600);
  Wire.begin();
  delay(500);

// Set pin mode of motor controls and push button 
  pinMode(ENABLE, OUTPUT);
  pinMode(DIRA, OUTPUT);
  pinMode(DIRB, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
}


void RTC() {

// Retreive time information from RTC
  DateTime now = myRTC.now();

// Compensate for time difference and keep 12 hour format
  hour = now.hour() - 1;
  if(hour > 12){
  hour = hour % 12;
  }
  minute = now.minute() - 3;
  second = now.second();

// set the cursor to column 9, line 1
  lcd.setCursor(9, 1);

// print the stored time values
  lcd.print(hour);
  lcd.print(":");

  if (minute < 10) {
//lcd.print("0");
    lcd.print(minute);
  }

  else if (minute > 10) {
    lcd.print(minute);
  }

  lcd.print(":");

  if (second < 10) {
//lcd.print("0");
    lcd.print(second);
  }

  else if (second > 10) {
    lcd.print(second);
  }
  return NULL;
}


void rotate() {
  unsigned long currentMillis = millis();   //millis() returns run time ms

  Serial.print("Motor fn works");
  Serial.println();
  Serial.println(currentMillis);

// Input is read from the push button and toggles the value of the on variable 
  if ((digitalRead(buttonPin) == LOW) && on == true){
    Serial.print("Button Push");
    on = false;
  }

  else if((digitalRead(buttonPin) == LOW) && on == false){
    Serial.print("Button Push");
    on = true;
  }
  
// Millis will run this code in 5 second intervals
  if (currentMillis - previousMillis > interval) {
    previousMillis = currentMillis;
    Serial.println("Been 5 sec");
    count++;
    result = count % 4;
    Serial.println(result);
  }
    // CCW direction
  if (on == true) {
    digitalWrite(ENABLE, HIGH); // enable on
    digitalWrite(DIRA, LOW);
    digitalWrite(DIRB, HIGH);
    lcd.setCursor(11,0);
    lcd.print("CC");
  }
  
   // CW direction
  if (on == false) {
    digitalWrite(DIRA, HIGH);
    digitalWrite(DIRB, LOW);
    lcd.setCursor(11,0);
    lcd.print("CW");
    }

  lcd.setCursor(3, 0); //Set position for printing to the LCD
  
  // Speed of Motor is changed and printed onto LCD
  if(result == 0){
    Serial.println("MAX SPEED");
    lcd.clear();
    lcd.print("Full Spd");
    digitalWrite(ENABLE,255);   // MAX
  }
  
  if(result == 1){
    Serial.println("3/4 SPEED");
    lcd.clear();
    lcd.print("3/4 Spd");
    digitalWrite(ENABLE,192);   // 3/4 
  }
    
  if(result == 2){
    Serial.println("1/2 SPEED");
    lcd.clear();
    lcd.print("1/2 Spd");
    digitalWrite(ENABLE,128);   // 1/2 
    }
    
  if(result == 3){
    Serial.println("LOW SPEED");
    lcd.clear();
    lcd.print("Zero Spd");     // 0
    digitalWrite(DIRA, LOW);
    digitalWrite(DIRB, HIGH);
    digitalWrite(DIRA, HIGH);
    digitalWrite(DIRB, LOW);
  }

}

// Void loop repeatedly calls functions and mimics synchronous operation
void loop(){
  rotate();
  RTC();
}
