#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_MLX90614.h>

#define IR_sensor 7
#define PUMP 12
#define LOCK 10

Adafruit_SSD1306 display(-1);
Adafruit_MLX90614 mlx = Adafruit_MLX90614();
void greetings(void);
void thermal_screening(void);
void sanitization(void);
void unlock(void);
void contact(void);

void greetings(){
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(23, 12);
  display.print("WELCOME");
  display.display();
  delay(2500);
  display.clearDisplay();
}

void contact()
{
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(4, 4);
  display.print("Please contact");
  display.setCursor(32, 20);
  display.print("90519*****");
  display.display();
 
  char ch=Serial.read();
  if(ch=='g'){ // g from user means unknown person is allowed to proceed further
  thermal_screening();
  sanitization();
  unlock();
  }
 }

void thermal_screening() {
  String temp;
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(4, 4);
  display.print("Place fist before");
  display.setCursor(4, 18);
  display.print("Temp Sensor");
  display.display();

  delay(4000);

  display.clearDisplay();
  delay(200);
  mlx.begin();
  float temp_value = mlx.readObjectTempF();
  if(temp_value>98.6)
  Serial.write('F'); // F stands for person having fever
  else
  Serial.write('N'); // N stands for person with no fever

  display.setCursor(4, 6);
  display.print("Your Temperature:");
  display.setCursor(30, 18);
  display.print(temp_value);
  display.print(" deg F");
  display.display();
  delay(2500);
  display.clearDisplay();
}

void sanitization(){
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(4, 1);
  display.print("Place your palms");
  display.setCursor(4, 13);
  display.print("under sanitization");
  display.setCursor(4, 22);
  display.print("nozzle and sanitize");
  display.display();
  delay(3000);
  int irsensor= digitalRead(IR_sensor);
  if(irsensor==1){
  digitalWrite(PUMP, HIGH);
  delay(400);
  digitalWrite(PUMP, LOW);
  Serial.write('d'); // d stands for sanitization complete
  }
  delay(1000);
  display.clearDisplay();
  
  
}

void unlock(){
  char ch=Serial.read();
  if(ch=='o'){
  digitalWrite(LOCK, HIGH);
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(23, 14);
  display.print("Door Unlocked");
  display.display();
  delay(5000);
  display.clearDisplay();
  }
  if(ch=='f')
  digitalWrite(LOCK, LOW);
  else
  digitalWrite(LOCK, LOW);
}

void setup() {
  Serial.begin(115200);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  pinMode(IR_sensor, INPUT);
  pinMode(PUMP, OUTPUT);
  pinMode(LOCK, OUTPUT);
}

void loop() {
  if (Serial.available() > 0){
    char s1 = Serial.read();
    greetings();
    delay(500);
    if (s1 == '1') {
      thermal_screening();
      delay(500);
      sanitization();
      delay(500);
      unlock();
      delay(1000);
    }
    if (s1 == '2') {
      sanitization();
      delay(500);
      unlock();
      delay(1000);
    }
    if (s1 == '3') {
      contact();
      delay(1000);

    }
    if (s1 == '4') {
      unlock();
    }
  }
  delay(100);

}