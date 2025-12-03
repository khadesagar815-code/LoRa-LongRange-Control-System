//http://dan.drown.org/stm32duino/package_STM32duino_index.json

#include <SPI.h>
#include <LoRa.h>

// LoRa module connections
#define LORA_SS    PA4
#define LORA_RST   PB0
#define LORA_DIO0  PB11

// Onboard LED (Blue Pill LED is usually on PC13)
#define LED_PIN    PC13

String receivedText = "";

// Callback when a LoRa packet is received
void onReceive(int packetSize) {
  if (packetSize == 0) return;

  receivedText = "";
  while (LoRa.available()) {
    char c = (char)LoRa.read();
    receivedText += c;
  }

  receivedText.trim();  // Remove whitespace and newline

  Serial.print("Received: ");
  Serial.print(receivedText);
  Serial.print(" | RSSI: ");
  Serial.println(LoRa.packetRssi());

  // Check for LED commands
  if (receivedText.equalsIgnoreCase("ON")) {
    digitalWrite(LED_PIN, LOW); // LOW turns on LED on Blue Pill
    Serial.println("LED ON");
  } else if (receivedText.equalsIgnoreCase("OFF")) {
    digitalWrite(LED_PIN, HIGH); // HIGH turns off LED on Blue Pill
    Serial.println("LED OFF");
  }
}

void setup() {
  Serial.begin(9600);
//  while (!Serial);

  Serial.println("LoRa Receiver with LED Control");

  // Reset LoRa module
  pinMode(LORA_RST, OUTPUT);
  digitalWrite(LORA_RST, LOW);
  delay(100);
  digitalWrite(LORA_RST, HIGH);
  delay(100);

  // Set LoRa module pins
  LoRa.setPins(LORA_SS, LORA_RST, LORA_DIO0);

  if (!LoRa.begin(433E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }

  // Initialize onboard LED
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH); // Turn OFF initially (LED is active LOW)

  // Set receive callback and enter receive mode
  LoRa.onReceive(onReceive);
  LoRa.receive();

  Serial.println("Receiver ready. Send 'ON' or 'OFF' to control LED.");
}

void loop() {
  Serial.println(".");
  delay(5000);
  // Nothing needed in loop
}
