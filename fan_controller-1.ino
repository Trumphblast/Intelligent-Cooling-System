#include <DHT.h>

#define DHTPIN 4        // DHT signal pin connected to GPIO 4
#define DHTTYPE DHT11   // or DHT22
#define RELAY_PIN 5     // Relay control pin

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);

  dht.begin();

  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, HIGH); // relay OFF if active LOW
}

void loop() {
  float temp = dht.readTemperature(); // Celsius

  if (isnan(temp)) {
    Serial.println("Failed to read from DHT!");
    return;
  }

  Serial.print("Temperature: ");
  Serial.print(temp);
  Serial.println(" °C");

  // --- Control Fan ---
  if (temp > 30) {
    digitalWrite(RELAY_PIN, LOW);   // turn relay ON (active LOW)
    Serial.println("Fan ON");
  } 
  else if (temp < 28) {
    digitalWrite(RELAY_PIN, HIGH);  // turn relay OFF
    Serial.println("Fan OFF");
  }

  delay(2000);  // wait 2 sec before next reading
}
