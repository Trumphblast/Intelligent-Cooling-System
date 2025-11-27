/*************************************************
   Blynk Template Settings
 *************************************************/
#define BLYNK_TEMPLATE_ID ""
#define BLYNK_TEMPLATE_NAME ""
#define BLYNK_AUTH_TOKEN ""

/*************************************************
   WiFi Credentials  → ADD YOURS HERE
 *************************************************/
char ssid[] = "SSID";
char pass[] = "Password";

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

#define RELAY_PIN 5     // Relay controlling the fan

DHT dht(DHTPIN, DHTTYPE);
BlynkTimer timer;

/*************************************************
   Virtual Pins (must match your Blynk Datastreams)
 *************************************************/
#define V_TEMP V1
#define V_HUM  V2
#define V_MANUAL V3
#define V_FEEDBACK V4

/*************************************************
   Fan Control Variables
 *************************************************/
bool manualOverride = false;
int manualState = 0;        // 0 = off, 1 = on

bool fanAutoState = false;  // internal automatic state

float ON_TEMP = 23.0;       // fan turns on above 30°C
float OFF_TEMP = 20.0;      // fan turns off below 28°C

/*************************************************
   Read Sensors + Auto Logic
 *************************************************/
void sendReadings() {
  float t = dht.readTemperature();
  float h = dht.readHumidity();

  if (!isnan(t)) Blynk.virtualWrite(V_TEMP, t);
  if (!isnan(h)) Blynk.virtualWrite(V_HUM, h);

  if (!manualOverride) {
    // --- AUTOMATIC CONTROL ---
    if (t > ON_TEMP) {
      fanAutoState = true;
    } else if (t < OFF_TEMP) {
      fanAutoState = false;
    }

    digitalWrite(RELAY_PIN, fanAutoState ? LOW : HIGH);
    Blynk.virtualWrite(V_FEEDBACK, fanAutoState ? 1 : 0);
  }
}

/*************************************************
   Manual Fan Control from App (Button on V3)
 *************************************************/
BLYNK_WRITE(V_MANUAL) {
  manualState = param.asInt();

  if (manualState == 2) {
    // RESET manual override (use this if you want a 3-state switch)
    manualOverride = false;
    return;
  }

  manualOverride = true; // user is controlling manually

  digitalWrite(RELAY_PIN, manualState ? LOW : HIGH);
  Blynk.virtualWrite(V_FEEDBACK, manualState);
}

/*************************************************
   Setup
 *************************************************/
void setup() {
  Serial.begin(115200);

  dht.begin();
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, HIGH);  // Relay OFF at boot

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  timer.setInterval(4000L, sendReadings); // send data every 4s
}

/*************************************************
   Loop
 *************************************************/
void loop() {
  Blynk.run();
  timer.run();
}
