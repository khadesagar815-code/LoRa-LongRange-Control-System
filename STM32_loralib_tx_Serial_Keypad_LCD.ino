#include <SPI.h>
#include <LoRa.h>

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// LCD address usually 0x27 or 0x3F, depends on backpack
LiquidCrystal_I2C lcd(0x27, 16, 2);  // 16x2 LCD


// LoRa Pins
#define LORA_SS    PA4
#define LORA_RST   PB0
#define LORA_DIO0  PA1

// Keypad pins (using PA8 as COM)
#define KEY1 PB13
#define KEY2 PB12
#define KEY3 PB15
#define KEY4 PB14
#define KEY_COM PA8   // acts as GND

String inputBuffer = "";

void setup() {
  Serial.begin(9600);
  Serial.println("Start");

  lcd.init();        
  lcd.backlight();   

  // Reset LoRa
  pinMode(LORA_RST, OUTPUT);
  digitalWrite(LORA_RST, LOW);
  delay(100);
  digitalWrite(LORA_RST, HIGH);
  delay(100);

  // LoRa init
  LoRa.setPins(LORA_SS, LORA_RST, LORA_DIO0);
  if (!LoRa.begin(433E6)) {
    Serial.println("LoRa init failed!");
    lcd.setCursor(0,0);
    lcd.print("LoRa init fail!");
    while (1);
  }
  Serial.println("LoRa initialized");
  lcd.setCursor(0,0);
  lcd.print("LoRa initialized");
  

  // Keypad setup
  pinMode(KEY1, INPUT_PULLUP);
  pinMode(KEY2, INPUT_PULLUP);
  pinMode(KEY3, INPUT_PULLUP);
  pinMode(KEY4, INPUT_PULLUP);

  pinMode(KEY_COM, OUTPUT);
  digitalWrite(KEY_COM, LOW);  // COM = GND
  delay(1000);
}

void loop() {
  
  if (digitalRead(KEY1) == LOW) {
    inputBuffer += "1";
    Serial.print("1");
    delay(300);  // debounce
  }
  if (digitalRead(KEY2) == LOW) {
    inputBuffer += "2";
    Serial.print("2");
    delay(300);
  }
  if (digitalRead(KEY3) == LOW) {
    inputBuffer += "3";
    Serial.print("3");
    delay(300);
  }
  if (digitalRead(KEY4) == LOW) {
    if (inputBuffer.length() > 0) {
      // Send buffer via LoRa
      LoRa.beginPacket();
      LoRa.print(inputBuffer);
      LoRa.endPacket();

      Serial.println("Sent: " + inputBuffer);
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Sent: " + inputBuffer);
      inputBuffer = ""; // clear buffer
    } else {
      Serial.println("Nothing to send");
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Nothing to send");
    }
    delay(1000);
  }

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Enter Code:");
  lcd.setCursor(0,1);
  lcd.print(inputBuffer);
  delay(300);
}
