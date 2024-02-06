#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>  
#include <Adafruit_Sensor.h>
#include "DHT.h"

//declaration of fan 
const int PWM_pin = 3;
int count = 0;
int rpm;
unsigned long start_time;
//fan declaration ends here

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define OLED_RESET 4
//Adafruit_SH1106 display(OLED_RESET);
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT);  
#define DHTPIN 8     // Digital pin connected to the DHT sensor

#define BLUE 0x001F

// Uncomment the type of sensor in use:
//#define DHTTYPE    DHT11     // DHT 11
#define DHTTYPE    DHT22     // DHT 22 (AM2302)
//#define DHTTYPE    DHT21     // DHT 21 (AM2301)

DHT dht(DHTPIN, DHTTYPE);

//set threshold humidity above which fan speed increases
float threshold_hum = 58.30;

void setup() {
  Serial.begin(9600);
  dht.begin();

  //pinMode for fan declaration
  pinMode(PWM_pin, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(2), counter, RISING);
  //pinMode ends here

 // display.begin(SH1106_SWITCHCAPVCC, 0x3C);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  
  delay(2000);
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  //display.setTextColor(BLUE);
}

void loop() {
  //delay(1000);

  //read temperature and humidity
  float t = dht.readTemperature();
  float h = dht.readHumidity();

  //checking if the sensor is working
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!"); //if sensor fail to return readings, display this message
  }


//fans regulation process
  int PMWVal;

  PMWVal = map(h, threshold_hum, 100, 20, 255);

  analogWrite(PWM_pin, PMWVal);


  start_time = millis();
  count = 0;
  while((millis() - start_time) < 1000){

  }
  
  rpm = count * 30;

  float velocity = (2 * (22/7) * 0.057 * rpm) / 60;
  Serial.println(velocity);
//fan regulation ends here



//print the temperature and humidity readings
  //Serial.println(t);
  //Serial.println(h);

//clear display
  display.clearDisplay();

// display temperature
  display.setTextSize(1);
  display.setCursor(0,7);
  //display.setTextColor(BLUE);
  display.print("Temperature: ");
  display.setTextSize(2);
  display.setCursor(0,16);
  display.print(t);
  display.print(" ");
  display.setTextSize(1);
  display.cp437(true);
  display.write(167);
  display.setTextSize(2);
  display.print("C");
  
  // display humidity
  display.setTextSize(1);
  display.setCursor(0, 37);
  display.print("Humidity: ");
  display.setTextSize(2);
  display.setCursor(0, 50);
  display.print(h);
  display.print(" %"); 
  
  display.display(); 
}


void counter(){
  count ++;
}