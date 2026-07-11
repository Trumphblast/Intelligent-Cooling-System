#include <DHT.h>

#define DHTPIN 4
#define DHTTYPE DHT11
#define RELAY_PIN 5

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);

  dht.begin();

  pinMode(RELAY_PIN, OUTPUT);

  // Active LOW relay
  digitalWrite(RELAY_PIN, HIGH);

  delay(2000);   // Allow DHT to stabilize
}

void loop() {

  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor!");
    delay(2000);
    return;
  }

  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print(" °C");

  Serial.print("   Humidity: ");
  Serial.print(humidity);
  Serial.println(" %");

  // Relay Control
  if (temperature >= 30.0) {
    digitalWrite(RELAY_PIN, LOW);      // Fan ON
    Serial.println("Fan ON");
  }
  else if (temperature <= 28.0) {
    digitalWrite(RELAY_PIN, HIGH);     // Fan OFF
    Serial.println("Fan OFF");
  }

  delay(2000);
}
