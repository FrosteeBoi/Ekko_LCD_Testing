
/*
  LCD + DHT11 Temperature and Humidity Display

  Based on the Elegoo LiquidCrystal "Hello World" example.

  The circuit:
  LCD RS pin to digital pin 7
  LCD Enable pin to digital pin 8
  LCD D4 pin to digital pin 9
  LCD D5 pin to digital pin 10
  LCD D6 pin to digital pin 11
  LCD D7 pin to digital pin 12
  LCD R/W pin to ground
  LCD VSS pin to ground
  LCD VCC pin to 5V
  10K potentiometer: ends to +5V and ground, wiper to LCD VO pin (pin 3)

  DHT11 signal (data) pin to digital pin 2
  DHT11 VCC to 5V, GND to ground
  (bare 4-pin DHT11 needs a 10K pull-up between data and VCC;
   3-pin breakout modules already have one)

  Requires the "DHT sensor library" by Adafruit
  (plus "Adafruit Unified Sensor", which it depends on).
*/

#include <LiquidCrystal.h>
#include <DHT.h>

#define DHTPIN 2        // DHT11 data pin
#define DHTTYPE DHT11   // sensor type
#define BUTTON1 3
float capacity = 5.0;   // Ah
float soc = 100.0; // initial charge percent
float current = 20.0; // amperes
float dt = 1.0 / 3600.0; // 1 second in hours

int screen = 0;
int last_button_state = HIGH;


DHT dht(DHTPIN, DHTTYPE);
unsigned long last_battery_update = 0;

// initialize the LCD with the numbers of the interface pins
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

void setup() {
  lcd.begin(16, 2);
  dht.begin();
  pinMode(BUTTON1, INPUT_PULLUP);

  lcd.print("Reading sensor..");
}



void loop() {
  int button_state = digitalRead(BUTTON1);
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();   // Celsius
  if (millis() - last_battery_update >= 1000) {
    last_battery_update = millis();

    soc -= (100.0 / capacity) * current * dt;

    if (soc < 0) {
        soc = 0;
    }
}

  // Detect a button press
  if (button_state == LOW &&
    last_button_state == HIGH) {

    screen++;

    if (screen > 1) {
        screen = 0;
    }
    lcd.clear();
    delay(50); // Simple debounce
    }

    last_button_state = button_state;

    if (screen == 0) {
      // If a read failed, show an error instead of garbage values
      if (isnan(humidity) || isnan(temperature)) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Sensor error!");
        lcd.setCursor(0, 1);
        lcd.print("Check wiring");
        return;
      }

      // Row 0: temperature
      lcd.setCursor(0, 0);
      lcd.print("Temp: ");
      lcd.print(temperature, 1);   // one decimal place
      lcd.write(223);              // degree symbol on HD44780 displays
      lcd.print("C   ");           // trailing spaces wipe leftover characters

      // Row 1: humidity
      lcd.setCursor(0, 1);
      lcd.print("Humidity: ");
      lcd.print(humidity, 0);      // DHT11 only has 1% resolution
      lcd.print("%   ");

    }
    else if (screen == 1) {
    lcd.setCursor(0, 0);
    lcd.print("Battery: ");
    lcd.print(soc, 1);
    lcd.print("%   ");

    lcd.setCursor(0, 1);
    lcd.print("Current: ");
    lcd.print(current, 1);
    lcd.print("A  ");
}


  
 
  
}
