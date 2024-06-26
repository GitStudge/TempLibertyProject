//Includes all libraries necessary for proper function
#include <LiquidCrystal_I2C.h>
#include <Adafruit_NeoPixel.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Wire.h>

#define LED_PIN 12      // Pin connected to the LED Strip
#define NUM_LEDS 5      // Number of NeoPixels
#define BRIGHTNESS 200  // Set brightness (0 to 255)
#define ONE_WIRE_BUS 2  // Pin connected to Sensor

LiquidCrystal_I2C lcd(0x27, 20, 4);
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

float Celsius = 0.0;
float Fahrenheit = 0.0;
float prevTemp = 0.0;
float lowerLimit = 0.0;
float upperLimit = 0.0;
int redColor = 0;
int greenColor = 0;
int blueColor = 0;
int temp = A3;

void setup() {

  pinMode(13, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(3, OUTPUT);
  digitalWrite(13, HIGH);
  digitalWrite(4, HIGH);
  digitalWrite(3, LOW);

  lcd.init();
  lcd.backlight();
  lcd.begin(20, 4);

  strip.begin();
  strip.setBrightness(BRIGHTNESS);
  strip.show();

  sensors.begin();
  Serial.begin(9600);
}

void loop() {
  sensors.requestTemperatures();
  Celsius = sensors.getTempCByIndex(0);
  Fahrenheit = sensors.toFahrenheit(Celsius);
  lowerLimit = prevTemp - 1.0;
  upperLimit = prevTemp + 1.0;

  lcd.setCursor(3, 0);
  lcd.print("Temp: " + String(Fahrenheit) + " F");
  if(Fahrenheit < 100) {
    lcd.setCursor(16, 0);
    for (int i = 0; i < 3; i++) {
      lcd.print(" ");
    }
  }
  if(Fahrenheit >= 100) {
    lcd.setCursor(17, 0);
    for (int i = 0; i < 4; i++) {
      lcd.print(" ");
    }
  }
  if (Fahrenheit < lowerLimit || Fahrenheit > upperLimit) {
    lcd.setCursor(5, 1);
    lcd.print("Sensing...");

    for(int i = 0; i <= NUM_LEDS; i++) {
      strip.setPixelColor(i, strip.Color(0, 0, 0));
      strip.show();
    }
    for(int i = 0; i <= NUM_LEDS; i++) {
      strip.setPixelColor(i, strip.Color(255, 150, 0));
      delay(250);
      strip.show();
      strip.setPixelColor(i, strip.Color(0, 0, 0));
      delay(200);
      strip.show();
    }
  }

if (Fahrenheit >= lowerLimit && Fahrenheit <= upperLimit) { //Checking to see if temperature is stable
  if (Fahrenheit < 98) {
    for (int i = 0; i <= NUM_LEDS; i++) {
      strip.setPixelColor(i, strip.Color(0, 0, 255));
      strip.show();
    }
    lcd.setCursor(5, 1);
    lcd.print("Cold Water");
  }

  if (Fahrenheit > 98 && Fahrenheit < 106) {
    for (int i = 0; i <= NUM_LEDS; i++) {
      strip.setPixelColor(i, strip.Color(0, 255, 0));
      strip.show();
    }
    lcd.setCursor(5, 1);
    lcd.print("Warm Water");
  }

  if (Fahrenheit > 106 && Fahrenheit < 109) {
    for (int i = 0; i <= NUM_LEDS; i++) {
      strip.setPixelColor(i, strip.Color(255, 255, 0));
      strip.show();
    }
    lcd.setCursor(5, 1);
    lcd.print("Almost Hot");
  }

  if (Fahrenheit > 109) {
    for (int i = 0; i <= NUM_LEDS; i++) {
      strip.setPixelColor(i, strip.Color(255, 0, 0));
      strip.show();
    }
    lcd.setCursor(5, 1);
    lcd.print("Hot Water!");
  }
}
prevTemp = Fahrenheit; // Updating the previous temperature
}