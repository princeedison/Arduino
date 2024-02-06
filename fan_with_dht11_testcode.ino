// Example testing sketch for various DHT humidity/temperature sensors
// Tech at Home

#include "DHT.h"

#define DHTPIN 8     // what pin we're connected to

#define LED_PIN 9
const int PWM_pin = 3;
int count = 0;
int rpm;
unsigned long start_time;

// Uncomment whatever type you're using!
#define DHTTYPE DHT11   // DHT 11
//define DHTTYPE DHT22   // DHT 22  (AM2302)
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

// Connect pin 1 (on the left) of the sensor to +5V
// NOTE: If using a board with 3.3V logic like an Arduino Due connect pin 1
// to 3.3V instead of 5V!
// Connect pin 2 of the sensor to whatever your DHTPIN is
// Connect pin 4 (on the right) of the sensor to GROUND
// Connect a 10K resistor from pin 2 (data) to pin 1 (power) of the sensor

// Initialize DHT sensor.
// Note that older versions of this library took an optional third parameter to
// tweak the timings for faster processors.  This parameter is no longer needed
// as the current DHT reading algorithm adjusts itself to work on faster procs.
DHT dht(DHTPIN, DHTTYPE);

float threshold_temp = 31.30;

void setup() {
  Serial.begin(9600);
  Serial.println("DHTxx test!");

  pinMode(LED_PIN, OUTPUT);
  pinMode(PWM_pin, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(2), counter, RISING);

  dht.begin();
}

void loop() {
  // Wait a few seconds between measurements.
  //delay(10);

  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);

  if(t > threshold_temp){
    digitalWrite(LED_PIN, HIGH);
  } else {
    digitalWrite(LED_PIN, LOW);
  }

int PMWVal;

PMWVal = map(t, threshold_temp, 34.02, 20, 255);

analogWrite(PWM_pin, PMWVal);

  // if(t < threshold_Temp){
  //   analogWrite(PWM_pin, 40);
  // } else if((t >= threshold_Temp) && (t <= 31.20 )){
  //   analogWrite(PWM_pin, 126);
  // } else {
  //   analogWrite(PWM_pin, 255);
  // }

  start_time = millis();
  count = 0;
  while((millis() - start_time) < 1000){

  }
  
  rpm = count * 30;

  float velocity = (2 * (22/7) * 0.057 * rpm) / 60;

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Compute heat index in Fahrenheit (the default)
  float hif = dht.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(t, h, false);

  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.print(" *C ");
  Serial.print(f);
  Serial.print(" *F\t");
  Serial.print("Heat index: ");
  Serial.print(hic);
  Serial.print(" *C ");
  Serial.print(hif);
  Serial.print(" *F");
  Serial.print(" Fan speed is ");
  Serial.println(velocity);
}

void counter(){
  count ++;
}
