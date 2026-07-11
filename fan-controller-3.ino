/*************************************************
   Blynk Template Settings
 *************************************************/
#define BLYNK_TEMPLATE_ID ""
#define BLYNK_TEMPLATE_NAME ""
#define BLYNK_AUTH_TOKEN ""

/*************************************************
   WiFi Credentials
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
   Pin Configuration
 *************************************************/
#define DHTPIN 4
#define DHTTYPE DHT11

#define RELAY_PIN 5

DHT dht(DHTPIN, DHTTYPE);
BlynkTimer timer;

/*************************************************
   Virtual Pins
 *************************************************/
#define V_TEMP      V1
#define V_HUM       V2
#define V_MANUAL    V3
#define V_FEEDBACK  V4

/*************************************************
   Fan Control Variables
 *************************************************/
bool manualOverride = false;
bool fanState = false;

const float ON_TEMP = 23.0;     // Fan ON above 23°C
const float OFF_TEMP = 20.0;    // Fan OFF below 20°C

/*************************************************
   Update Relay
 *************************************************/
void setFan(bool state)
{
    fanState = state;

    // Active LOW relay
    digitalWrite(RELAY_PIN, state ? LOW : HIGH);

    Blynk.virtualWrite(V_FEEDBACK, state);
}

/*************************************************
   Read Sensors
 *************************************************/
void sendReadings()
{
    float temperature = dht.readTemperature();
    float humidity = dht.readHumidity();

    if (isnan(temperature) || isnan(humidity))
    {
        Serial.println("Failed to read DHT sensor!");
        return;
    }

    Blynk.virtualWrite(V_TEMP, temperature);
    Blynk.virtualWrite(V_HUM, humidity);

    if (!manualOverride)
    {
        if (temperature >= ON_TEMP)
        {
            setFan(true);
        }
        else if (temperature <= OFF_TEMP)
        {
            setFan(false);
        }
    }
}

/*************************************************
   Manual Control
 *************************************************/
BLYNK_WRITE(V_MANUAL)
{
    int value = param.asInt();

    manualOverride = true;

    if (value)
        setFan(true);
    else
        setFan(false);
}

/*************************************************
   Sync after reconnect
 *************************************************/
BLYNK_CONNECTED()
{
    Blynk.syncVirtual(V_MANUAL);
    Blynk.virtualWrite(V_FEEDBACK, fanState);
}

/*************************************************
   Setup
 *************************************************/
void setup()
{
    Serial.begin(115200);

    pinMode(RELAY_PIN, OUTPUT);

    // Relay OFF
    digitalWrite(RELAY_PIN, HIGH);

    dht.begin();

    Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

    timer.setInterval(4000L, sendReadings);
}

/*************************************************
   Main Loop
 *************************************************/
void loop()
{
    Blynk.run();
    timer.run();
}
