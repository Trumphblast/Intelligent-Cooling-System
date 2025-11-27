/*************************************************
   Blynk Template Settings
 *************************************************/
#define BLYNK_TEMPLATE_ID "TMPL4n3wdaUF0"
#define BLYNK_TEMPLATE_NAME "ESP32 Fan Controller"
#define BLYNK_AUTH_TOKEN "DFYMKy3kMRZM7kwXeA9OzhuqoVDhYOky"

/*************************************************
   WiFi Credentials
 *************************************************/
char ssid[] = "SSID";           
char pass[] = "password";      

/*************************************************
   Includes
 *************************************************/
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <DHT.h>

/*************************************************
   Pin Config
 *************************************************/
#define DHTPIN 4
#define DHTTYPE DHT11

#define RELAY_PIN 5    

DHT dht(DHTPIN, DHTTYPE);
BlynkTimer timer;

/*************************************************
   Blynk Virtual Pins
 *************************************************/
#define V_TEMP     V1
#define V_HUM      V2
#define V_MANUAL   V3
#define V_FEEDBACK V4   

/*************************************************
   Fan Control Variables
 *************************************************/
bool manualOverride = false;
int manualState = 0;        

bool fanAutoState = false;

float ON_TEMP = 30.0;     // Auto ON temperature threshold
float OFF_TEMP = 25.0;    // Auto OFF temperature threshold

/*************************************************
   Read Sensors + Automatic Fan Logic
   - Always prints to Serial
   - Sends to Blynk only if connected
 *************************************************/
void sendReadings() {
  float t = dht.readTemperature();
  float h = dht.readHumidity();

  // Serial debug always
  if (isnan(t) || isnan(h)) {
    Serial.println("[DHT] Failed to read sensor!");
  } else {
    Serial.print("[DHT] Temperature: ");
    Serial.print(t);
    Serial.print(" °C  | Humidity: ");
    Serial.print(h);
    Serial.println(" %");
  }

  // Send to Blynk only if connected
  if (Blynk.connected()) {
    if (!isnan(t)) Blynk.virtualWrite(V_TEMP, t);
    if (!isnan(h)) Blynk.virtualWrite(V_HUM, h);
  } else {
    Serial.println("[Blynk] Not connected — skipping virtualWrite.");
  }

  // Automatic control if not overridden
  if (!manualOverride && !isnan(t)) {
    if (t > ON_TEMP) {
      fanAutoState = true;
    } else if (t < OFF_TEMP) {
      fanAutoState = false;
    }
    digitalWrite(RELAY_PIN, fanAutoState ? LOW : HIGH);
    // Update feedback only if connected
    if (Blynk.connected()) Blynk.virtualWrite(V_FEEDBACK, fanAutoState ? 1 : 0);
    Serial.print("[FAN] Auto mode, fanAutoState=");
    Serial.println(fanAutoState ? "ON" : "OFF");
  }
}

/*************************************************
   Manual Fan Switch from Blynk (V3)
 *************************************************/
BLYNK_WRITE(V_MANUAL) {
  manualState = param.asInt();
  Serial.print("[BLYNK] Manual V3 value: ");
  Serial.println(manualState);

  if (manualState == 2) {
    manualOverride = false;   // Reset to auto mode if you use 3-state button
    Serial.println("[FAN] Manual override cleared (back to AUTO).");
    return;
  }

  manualOverride = true;      // User activated manual control
  digitalWrite(RELAY_PIN, manualState ? LOW : HIGH);
  if (Blynk.connected()) Blynk.virtualWrite(V_FEEDBACK, manualState);
  Serial.print("[FAN] Manual override, fan is now ");
  Serial.println(manualState ? "ON" : "OFF");
}

/*************************************************
   Setup
 *************************************************/
void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("\n--- Booting ESP32 Fan Controller ---");

  dht.begin();
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, HIGH);  // Relay OFF at boot

  Serial.print("[WiFi] Connecting to: ");
  Serial.println(ssid);

  // Use Blynk.begin (blocking) but show debug messages
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  // After Blynk.begin returns, show status
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("[WiFi] Connected.");
  } else {
    Serial.println("[WiFi] Not connected!");
  }

  if (Blynk.connected()) {
    Serial.println("[Blynk] Connected to Blynk Cloud.");
  } else {
    Serial.println("[Blynk] NOT connected to Blynk Cloud.");
  }

  // Do one immediate reading so you see Serial output quickly
  sendReadings();

  // schedule periodic readings
  timer.setInterval(4000L, sendReadings);
}

/*************************************************
   Loop
 *************************************************/
void loop() {
  Blynk.run();
  timer.run();
}
